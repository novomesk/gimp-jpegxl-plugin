
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <glib/gstdio.h>

#include <jxl/decode.h>
#include <jxl/encode.h>
#include <jxl/thread_parallel_runner.h>

#define LOAD_PROC       "file-jpegxl-load"
#define SAVE_PROC       "file-jpegxl-save"
#define PLUG_IN_BINARY  "file-jpegxl"

typedef struct _JpegXL      JpegXL;
typedef struct _JpegXLClass JpegXLClass;

struct _JpegXL
{
  GimpPlugIn      parent_instance;
};

struct _JpegXLClass
{
  GimpPlugInClass parent_class;
};


#define JPEGXL_TYPE  (jpegxl_get_type ())
#define JPEGXL (obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), JPEGXL_TYPE, JpegXL))

GType                   jpegxl_get_type (void) G_GNUC_CONST;

static GList           *jpegxl_query_procedures (GimpPlugIn           *plug_in);
static GimpProcedure   *jpegxl_create_procedure (GimpPlugIn           *plug_in,
    const gchar          *name);

static GimpValueArray *jpegxl_load (GimpProcedure        *procedure,
                                    GimpRunMode           run_mode,
                                    GFile                *file,
                                    const GimpValueArray *args,
                                    gpointer              run_data);
static GimpValueArray *jpegxl_save (GimpProcedure        *procedure,
                                    GimpRunMode           run_mode,
                                    GimpImage            *image,
                                    gint                  n_drawables,
                                    GimpDrawable        **drawables,
                                    GFile                *file,
                                    const GimpValueArray *args,
                                    gpointer              run_data);


G_DEFINE_TYPE (JpegXL, jpegxl, GIMP_TYPE_PLUG_IN)

GIMP_MAIN (JPEGXL_TYPE)

static void
jpegxl_class_init (JpegXLClass *klass)
{
  GimpPlugInClass *plug_in_class = GIMP_PLUG_IN_CLASS (klass);

  plug_in_class->query_procedures = jpegxl_query_procedures;
  plug_in_class->create_procedure = jpegxl_create_procedure;
}

static void
jpegxl_init (JpegXL *jpeg_xl)
{
}

static GList *
jpegxl_query_procedures (GimpPlugIn *plug_in)
{
  GList *list = NULL;

  list = g_list_append (list, g_strdup (LOAD_PROC));
  list = g_list_append (list, g_strdup (SAVE_PROC));

  return list;
}

static GimpProcedure *
jpegxl_create_procedure (GimpPlugIn  *plug_in,
                         const gchar *name)
{
  GimpProcedure *procedure = NULL;

  if (! strcmp (name, LOAD_PROC))
    {
      procedure = gimp_load_procedure_new (plug_in, name,
                                           GIMP_PDB_PROC_TYPE_PLUGIN,
                                           jpegxl_load, NULL, NULL);

      gimp_procedure_set_menu_label (procedure, "JPEG-XL image");

      gimp_procedure_set_documentation (procedure,
                                        "Loads files in the JPEG-XL file format",
                                        "Loads files in the JPEG-XL file format",
                                        name);
      gimp_procedure_set_attribution (procedure,
                                      "Daniel Novomesky",
                                      "(C) 2020 Daniel Novomesky",
                                      "2021");

      gimp_file_procedure_set_mime_types (GIMP_FILE_PROCEDURE (procedure),
                                          "image/jxl");
      gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure),
                                          "jxl");
      gimp_file_procedure_set_magics (GIMP_FILE_PROCEDURE (procedure),
                                      "0,string,\xFF\x0A,0,string,3,string,\x0CJXL \x0D\x0A\x87\x0A");

    }
  else if (! strcmp (name, SAVE_PROC))
    {
      procedure = gimp_save_procedure_new (plug_in, name,
                                           GIMP_PDB_PROC_TYPE_PLUGIN,
                                           jpegxl_save, NULL, NULL);

      gimp_procedure_set_image_types (procedure, "RGB*, GRAY*");

      gimp_procedure_set_menu_label (procedure, "JPEG-XL image");

      gimp_procedure_set_documentation (procedure,
                                        "Saves files in the JPEG-XL file format",
                                        "Saves files in the JPEG-XL file format",
                                        name);
      gimp_procedure_set_attribution (procedure,
                                      "Daniel Novomesky",
                                      "(C) 2020 Daniel Novomesky",
                                      "2021");

      gimp_file_procedure_set_format_name (GIMP_FILE_PROCEDURE (procedure),
                                           "JPEG-XL");
      gimp_file_procedure_set_mime_types (GIMP_FILE_PROCEDURE (procedure),
                                          "image/jxl");
      gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure),
                                          "jxl");

      GIMP_PROC_ARG_BOOLEAN (procedure, "lossless",
                             "L_ossless",
                             "Use lossless compression",
                             FALSE,
                             G_PARAM_READWRITE);

      GIMP_PROC_ARG_DOUBLE (procedure, "compression",
                            "Co_mpression/maxError",
                            "Max. butteraugli distance, lower = higher quality. Range: 0 .. 15. 1.0 = visually lossless.",
                            0, 15, 1,
                            G_PARAM_READWRITE);

    }

  return procedure;
}

static GimpImage *load_image (GFile        *file,
                              GimpRunMode   runmode,
                              GError      **error)
{
  gchar            *filename = g_file_get_path (file);

  FILE *inputFile = g_fopen (filename, "rb");

  gsize inputFileSize;
  gpointer memory;
  JxlSignature signature;
  JxlDecoder *decoder;
  void *runner;
  JxlBasicInfo basicinfo;
  JxlDecoderStatus status;
  JxlPixelFormat pixel_format;
  size_t icc_size = 0;
  GimpColorProfile *profile = NULL;
  gboolean loadlinear = FALSE;
  size_t result_size;
  gpointer picture_buffer;

  GimpImage        *image;
  GimpLayer        *layer;
  GeglBuffer       *buffer;

  if (!inputFile)
    {
      g_message ("Cannot open file for read: %s\n", filename);
      g_free (filename);
      return NULL;
    }

  fseek (inputFile, 0, SEEK_END);
  inputFileSize = ftell (inputFile);
  fseek (inputFile, 0, SEEK_SET);

  if (inputFileSize < 1)
    {
      g_message ("File too small: %s\n", filename);
      fclose (inputFile);
      g_free (filename);
      return NULL;
    }

  memory = g_malloc (inputFileSize);
  if (fread (memory, 1, inputFileSize, inputFile) != inputFileSize)
    {
      g_message ("Failed to read %zu bytes: %s\n", inputFileSize, filename);
      fclose (inputFile);
      g_free (memory);

      g_free (filename);
      return NULL;
    }

  fclose (inputFile);

  signature = JxlSignatureCheck (memory, inputFileSize);
  if (signature != JXL_SIG_CODESTREAM && signature != JXL_SIG_CONTAINER)
    {
      g_message ("File %s is probably not in JXL format!\n", filename);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  decoder = JxlDecoderCreate (NULL);
  if (!decoder)
    {
      g_message ("ERROR: JxlDecoderCreate failed");
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  runner = JxlThreadParallelRunnerCreate (NULL, gimp_get_num_processors());
  if (JxlDecoderSetParallelRunner (decoder, JxlThreadParallelRunner, runner) != JXL_DEC_SUCCESS)
    {
      g_message ("ERROR: JxlDecoderSetParallelRunner failed");
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  if (JxlDecoderSetInput (decoder, memory, inputFileSize) != JXL_DEC_SUCCESS)
    {
      g_message ("ERROR: JxlDecoderSetInput failed");
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  if (JxlDecoderSubscribeEvents (decoder, JXL_DEC_BASIC_INFO | JXL_DEC_COLOR_ENCODING | JXL_DEC_FULL_IMAGE) != JXL_DEC_SUCCESS)
    {
      g_message ("ERROR: JxlDecoderSubscribeEvents failed");
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  status = JxlDecoderProcessInput (decoder);
  if (status == JXL_DEC_ERROR)
    {
      g_message ("ERROR: JXL decoding failed");
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  if (status == JXL_DEC_NEED_MORE_INPUT)
    {
      g_message ("ERROR: JXL data incomplete");
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  status = JxlDecoderGetBasicInfo (decoder, &basicinfo);
  if (status != JXL_DEC_SUCCESS)
    {
      g_message ("ERROR: JXL basic info not available");
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  if (basicinfo.xsize == 0 || basicinfo.ysize == 0)
    {
      g_message ("ERROR: JXL image has zero dimensions");
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  status = JxlDecoderProcessInput (decoder);
  if (status != JXL_DEC_COLOR_ENCODING)
    {
      g_message ("Unexpected event %d instead of JXL_DEC_COLOR_ENCODING", status);
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  pixel_format.endianness = JXL_NATIVE_ENDIAN;
  pixel_format.align = 0;
  pixel_format.data_type = JXL_TYPE_FLOAT;

  if (basicinfo.num_color_channels == 1) /* grayscale */
    {
      if (basicinfo.alpha_bits > 0)
        {
          pixel_format.num_channels = 2;
        }
      else
        {
          pixel_format.num_channels = 1;
        }
    }
  else /* RGB */
    {

      if (basicinfo.alpha_bits > 0) /* RGB with alpha */
        {
          pixel_format.num_channels = 4;
        }
      else /* RGB no alpha */
        {
          pixel_format.num_channels = 3;
        }
    }

  result_size = 4 * pixel_format.num_channels * (size_t) basicinfo.xsize * (size_t) basicinfo.ysize;

  if (JxlDecoderGetICCProfileSize (decoder, &pixel_format, JXL_COLOR_PROFILE_TARGET_DATA, &icc_size) == JXL_DEC_SUCCESS)
    {
      if (icc_size > 0)
        {
          gpointer raw_icc_profile = g_malloc (icc_size);

          if (JxlDecoderGetColorAsICCProfile (decoder, &pixel_format, JXL_COLOR_PROFILE_TARGET_DATA, raw_icc_profile, icc_size)
              == JXL_DEC_SUCCESS)
            {
              profile = gimp_color_profile_new_from_icc_profile (raw_icc_profile, icc_size, error);
              if (profile)
                {
                  loadlinear = gimp_color_profile_is_linear (profile);
                }
              else
                {
                  g_printerr ("%s: Failed to read ICC profile: %s\n", G_STRFUNC, (*error)->message);
                  g_clear_error (error);
                }
            }
          else
            {
              g_message ("Failed to obtain data from JPEG XL decoder");
            }

          g_free (raw_icc_profile);
        }
      else
        {
          g_message ("Empty ICC data");
        }
    }
  else
    {
      g_message ("no ICC, other color profile");
    }

  status = JxlDecoderProcessInput (decoder);
  if (status != JXL_DEC_NEED_IMAGE_OUT_BUFFER)
    {
      g_message ("Unexpected event %d instead of JXL_DEC_NEED_IMAGE_OUT_BUFFER", status);
      if (profile)
        {
          g_object_unref (profile);
        }
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  picture_buffer = g_malloc (result_size);
  if (JxlDecoderSetImageOutBuffer (decoder, &pixel_format, picture_buffer, result_size) != JXL_DEC_SUCCESS)
    {
      g_message ("ERROR: JxlDecoderSetImageOutBuffer failed");
      if (profile)
        {
          g_object_unref (profile);
        }
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }

  status = JxlDecoderProcessInput (decoder);
  if (status != JXL_DEC_FULL_IMAGE)
    {
      g_message ("Unexpected event %d instead of JXL_DEC_FULL_IMAGE", status);
      g_free (picture_buffer);
      if (profile)
        {
          g_object_unref (profile);
        }
      JxlThreadParallelRunnerDestroy (runner);
      JxlDecoderDestroy (decoder);
      g_free (memory);
      g_free (filename);
      return NULL;
    }


  if (basicinfo.num_color_channels == 1) /* grayscale */
    {
      image = gimp_image_new_with_precision (basicinfo.xsize, basicinfo.ysize, GIMP_GRAY, loadlinear ? GIMP_PRECISION_FLOAT_LINEAR : GIMP_PRECISION_FLOAT_NON_LINEAR);

      if (profile)
        {
          if (gimp_color_profile_is_gray (profile))
            {
              gimp_image_set_color_profile (image, profile);
            }
        }

      layer = gimp_layer_new (image, "Background",
                              basicinfo.xsize, basicinfo.ysize,
                              (basicinfo.alpha_bits > 0) ? GIMP_GRAYA_IMAGE : GIMP_GRAY_IMAGE, 100,
                              gimp_image_get_default_new_layer_mode (image));
    }
  else /* RGB */
    {
      image = gimp_image_new_with_precision (basicinfo.xsize, basicinfo.ysize, GIMP_RGB, loadlinear ? GIMP_PRECISION_FLOAT_LINEAR : GIMP_PRECISION_FLOAT_NON_LINEAR);

      if (profile)
        {
          if (gimp_color_profile_is_rgb (profile))
            {
              gimp_image_set_color_profile (image, profile);
            }
        }

      layer = gimp_layer_new (image, "Background",
                              basicinfo.xsize, basicinfo.ysize,
                              (basicinfo.alpha_bits > 0) ? GIMP_RGBA_IMAGE : GIMP_RGB_IMAGE, 100,
                              gimp_image_get_default_new_layer_mode (image));

    }

  gimp_image_insert_layer (image, layer, NULL, 0);

  buffer = gimp_drawable_get_buffer (GIMP_DRAWABLE (layer));

  gegl_buffer_set (buffer, GEGL_RECTANGLE (0, 0, basicinfo.xsize, basicinfo.ysize), 0,
                   NULL, picture_buffer, GEGL_AUTO_ROWSTRIDE);

  g_object_unref (buffer);


  g_free (picture_buffer);
  if (profile)
    {
      g_object_unref (profile);
    }
  JxlThreadParallelRunnerDestroy (runner);
  JxlDecoderDestroy (decoder);
  g_free (memory);
  g_free (filename);
  return image;
}

static GimpValueArray *
jpegxl_load (GimpProcedure        *procedure,
             GimpRunMode           run_mode,
             GFile                *file,
             const GimpValueArray *args,
             gpointer              run_data)
{
  GimpValueArray *return_vals;
  GimpImage      *image;
  GError         *error             = NULL;

  gegl_init (NULL, NULL);

  switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
    case GIMP_RUN_WITH_LAST_VALS:
      gimp_ui_init (PLUG_IN_BINARY);
      break;

    default:
      break;
    }

  image = load_image (file, run_mode, &error);

  if (! image)
    {
      return gimp_procedure_new_return_values (procedure,
             GIMP_PDB_EXECUTION_ERROR,
             error);
    }
  return_vals = gimp_procedure_new_return_values (procedure,
                GIMP_PDB_SUCCESS,
                NULL);

  GIMP_VALUES_SET_IMAGE (return_vals, 1, image);

  return return_vals;
}


static gboolean    save_image (GFile                *file,
                               GimpProcedureConfig *config,
                               GimpImage            *image,
                               GimpDrawable         *drawable,
                               GError              **error)
{
  JxlEncoder *encoder;
  void *runner;
  JxlEncoderOptions *encoder_options;
  JxlPixelFormat pixel_format;
  JxlBasicInfo output_info;
  JxlColorEncoding color_profile;
  JxlEncoderStatus status;
  size_t buffer_size;

  GByteArray *compressed;

  gchar          *filename;
  FILE           *outfile;
  GeglBuffer     *buffer;
  GimpImageType   drawable_type;

  gint drawable_width;
  gint drawable_height;
  gpointer picture_buffer;

  const Babl     *file_format = NULL;

  size_t offset = 0;
  uint8_t *next_out;
  size_t avail_out;

  gdouble compression = 1.0;
  gboolean lossless = FALSE;

  filename = g_file_get_path (file);
  gimp_progress_init_printf ("Exporting '%s'.", filename);

  g_object_get (config,
                "lossless",           &lossless,
                "compression",        &compression,
                NULL);

  drawable_type   = gimp_drawable_type (drawable);
  drawable_width  = gimp_drawable_get_width (drawable);
  drawable_height = gimp_drawable_get_height (drawable);

  memset (&output_info, 0, sizeof output_info);

  pixel_format.data_type = JXL_TYPE_UINT16;
  pixel_format.endianness = JXL_NATIVE_ENDIAN;
  pixel_format.align = 0;

  output_info.xsize = drawable_width;
  output_info.ysize = drawable_height;
  output_info.bits_per_sample = 16;
  output_info.exponent_bits_per_sample = 0;
  output_info.intensity_target = 255.0f;
  output_info.orientation = JXL_ORIENT_IDENTITY;

  switch (drawable_type)
    {
    case GIMP_GRAYA_IMAGE:
      file_format = babl_format ("Y'A u16");
      pixel_format.num_channels = 2;
      JxlColorEncodingSetToSRGB (&color_profile, JXL_TRUE);
      output_info.num_color_channels = 1;
      output_info.alpha_bits = 16;
      output_info.alpha_exponent_bits = 0;
      output_info.num_extra_channels = 1;
      break;
    case GIMP_GRAY_IMAGE:
      file_format = babl_format ("Y' u16");
      pixel_format.num_channels = 1;
      JxlColorEncodingSetToSRGB (&color_profile, JXL_TRUE);
      output_info.num_color_channels = 1;
      output_info.alpha_bits = 0;
      break;
    case GIMP_RGBA_IMAGE:
      file_format = babl_format ("R'G'B'A u16");
      pixel_format.num_channels = 4;
      JxlColorEncodingSetToSRGB (&color_profile, JXL_FALSE);
      output_info.num_color_channels = 3;
      output_info.alpha_bits = 16;
      output_info.alpha_exponent_bits = 0;
      output_info.num_extra_channels = 1;
      break;
    case GIMP_RGB_IMAGE:
      file_format = babl_format ("R'G'B' u16");
      pixel_format.num_channels = 3;
      JxlColorEncodingSetToSRGB (&color_profile, JXL_FALSE);
      output_info.num_color_channels = 3;
      output_info.alpha_bits = 0;
      break;
    default:
      g_free (filename);
      return FALSE;
      break;
    }


  buffer_size = 2 * pixel_format.num_channels * (size_t) output_info.xsize * (size_t) output_info.ysize;
  picture_buffer = g_malloc (buffer_size);

  gimp_progress_update (0.3);

  buffer = gimp_drawable_get_buffer (drawable);
  gegl_buffer_get (buffer, GEGL_RECTANGLE (0, 0,
                   drawable_width, drawable_height), 1.0,
                   file_format, picture_buffer,
                   GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);

  g_object_unref (buffer);

  gimp_progress_update (0.4);

  encoder = JxlEncoderCreate (NULL);
  if (!encoder)
    {
      g_message ("Failed to create Jxl encoder");
      g_free (picture_buffer);
      g_free (filename);
      return FALSE;
    }

  runner = JxlThreadParallelRunnerCreate (NULL, gimp_get_num_processors());
  if (JxlEncoderSetParallelRunner (encoder, JxlThreadParallelRunner, runner) != JXL_ENC_SUCCESS)
    {
      g_message ("JxlEncoderSetParallelRunner failed");
      JxlThreadParallelRunnerDestroy (runner);
      JxlEncoderDestroy (encoder);
      g_free (picture_buffer);
      g_free (filename);
      return FALSE;
    }

  status = JxlEncoderSetBasicInfo (encoder, &output_info);
  if (status != JXL_ENC_SUCCESS)
    {
      g_message ("JxlEncoderSetBasicInfo failed!");
      JxlThreadParallelRunnerDestroy (runner);
      JxlEncoderDestroy (encoder);
      g_free (picture_buffer);
      g_free (filename);
      return FALSE;
    }

  status = JxlEncoderSetColorEncoding (encoder, &color_profile);
  if (status != JXL_ENC_SUCCESS)
    {
      g_message ("JxlEncoderSetColorEncoding failed!");
      JxlThreadParallelRunnerDestroy (runner);
      JxlEncoderDestroy (encoder);
      g_free (picture_buffer);
      g_free (filename);
      return FALSE;
    }

  encoder_options = JxlEncoderOptionsCreate (encoder, NULL);

  if (lossless)
    {
      JxlEncoderOptionsSetDistance (encoder_options, 0);
      JxlEncoderOptionsSetLossless (encoder_options, JXL_TRUE);
    }
  else
    {
      JxlEncoderOptionsSetDistance (encoder_options, compression);
      JxlEncoderOptionsSetLossless (encoder_options, JXL_FALSE);
    }

  gimp_progress_update (0.5);

  status = JxlEncoderAddImageFrame (encoder_options, &pixel_format, picture_buffer, buffer_size);
  if (status == JXL_ENC_ERROR)
    {
      g_message ("JxlEncoderAddImageFrame failed!");
      JxlThreadParallelRunnerDestroy (runner);
      JxlEncoderDestroy (encoder);
      g_free (picture_buffer);
      g_free (filename);
      return FALSE;
    }

  JxlEncoderCloseInput (encoder);

  gimp_progress_update (0.7);

  compressed = g_byte_array_sized_new (4096);
  g_byte_array_set_size (compressed, 4096);
  do
    {
      next_out = compressed->data + offset;
      avail_out = compressed->len - offset;
      status = JxlEncoderProcessOutput (encoder, &next_out, &avail_out);

      if (status == JXL_ENC_NEED_MORE_OUTPUT)
        {
          offset = next_out - compressed->data;
          g_byte_array_set_size (compressed, compressed->len * 2);
        }
      else if (status == JXL_ENC_ERROR)
        {
          g_message ("JxlEncoderProcessOutput failed!");
          JxlThreadParallelRunnerDestroy (runner);
          JxlEncoderDestroy (encoder);
          g_free (picture_buffer);
          g_free (filename);
          return FALSE;
        }
    }
  while (status != JXL_ENC_SUCCESS);

  JxlThreadParallelRunnerDestroy (runner);
  JxlEncoderDestroy (encoder);

  g_free (picture_buffer);

  g_byte_array_set_size (compressed, next_out - compressed->data);

  gimp_progress_update (0.8);

  if (compressed->len > 0)
    {
      outfile = g_fopen (filename, "wb");
      if (!outfile)
        {
          g_message ("Could not open '%s' for writing!\n", filename);
          g_free (filename);
          g_byte_array_free (compressed, TRUE);
          return FALSE;
        }

      g_free (filename);
      fwrite (compressed->data, 1, compressed->len, outfile);
      fclose (outfile);

      gimp_progress_update (1.0);

      g_byte_array_free (compressed, TRUE);
      return TRUE;
    }
  g_byte_array_free (compressed, TRUE);
  g_free (filename);
  return FALSE;
}

static gboolean
save_dialog (GimpImage     *image,
             GimpProcedure *procedure,
             GObject       *config)
{
  GtkWidget    *dialog;
  gboolean      run;

  dialog = gimp_save_procedure_dialog_new (GIMP_SAVE_PROCEDURE (procedure),
           GIMP_PROCEDURE_CONFIG (config),
           image);

  gimp_procedure_dialog_get_widget (GIMP_PROCEDURE_DIALOG (dialog),
                                    "lossless", GTK_TYPE_CHECK_BUTTON);

  gimp_procedure_dialog_get_widget (GIMP_PROCEDURE_DIALOG (dialog),
                                    "compression", GIMP_TYPE_SCALE_ENTRY);

  gimp_procedure_dialog_fill (GIMP_PROCEDURE_DIALOG (dialog),
                              "lossless", "compression",
                              NULL);

  run = gimp_procedure_dialog_run (GIMP_PROCEDURE_DIALOG (dialog));

  gtk_widget_destroy (dialog);

  return run;
}


static GimpValueArray *
jpegxl_save (GimpProcedure        *procedure,
             GimpRunMode           run_mode,
             GimpImage            *image,
             gint                  n_drawables,
             GimpDrawable        **drawables,
             GFile                *file,
             const GimpValueArray *args,
             gpointer              run_data)
{
  GimpPDBStatusType      status = GIMP_PDB_SUCCESS;
  GimpProcedureConfig   *config;
  GimpExportReturn       export = GIMP_EXPORT_CANCEL;
  GError                *error  = NULL;

  gegl_init (NULL, NULL);

  config = gimp_procedure_create_config (procedure);
  gimp_procedure_config_begin_run (config, image, run_mode, args);

  switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
    case GIMP_RUN_WITH_LAST_VALS:
      gimp_ui_init (PLUG_IN_BINARY);

      export = gimp_export_image (&image, &n_drawables, &drawables, "JPEG XL",
                                  GIMP_EXPORT_CAN_HANDLE_RGB |
                                  GIMP_EXPORT_CAN_HANDLE_GRAY |
                                  GIMP_EXPORT_CAN_HANDLE_ALPHA);

      if (export == GIMP_EXPORT_CANCEL)
        {
          return gimp_procedure_new_return_values (procedure,
                 GIMP_PDB_CANCEL,
                 NULL);
        }
      break;

    default:
      break;
    }

  if (n_drawables < 1)
    {
      g_set_error (&error, G_FILE_ERROR, 0,
                   "No drawables to export");

      return gimp_procedure_new_return_values (procedure,
             GIMP_PDB_CALLING_ERROR,
             error);
    }

  if (run_mode == GIMP_RUN_INTERACTIVE)
    {
      if (! save_dialog (image, procedure, G_OBJECT (config)))
        {
          status = GIMP_PDB_CANCEL;
        }
    }

  if (status == GIMP_PDB_SUCCESS)
    {
      if (! save_image (file, config,
                        image, drawables[0], &error))
        {
          status = GIMP_PDB_EXECUTION_ERROR;
        }
    }


  gimp_procedure_config_end_run (config, status);
  g_object_unref (config);

  if (export == GIMP_EXPORT_EXPORT)
    {
      g_free (drawables);
      gimp_image_delete (image);
    }

  return gimp_procedure_new_return_values (procedure, status, error);
}
