# SimpleColorShifter
Color shift a bmp or ppm image based on user input


compilation: gcc CanellasImageProcessor.c BmpProcessor.c PpmProcessor.c Pixelprocessor.c -o imageprocessor


options:

  <input file name (required)>

  -o <outputfilename (required)>

  -r <amount of redshift (optional)>

  -g <amount of greenshift (optional)>

  -b <amount of blueshift (optional)>

  -t <output file format | only valid options are BMP and PPM>


example: ./imageprocessor inputimage.bmp -o outputimage.ppm -r 40 -g 200 -b 255 -t PPM
