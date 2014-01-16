README.txt on machine-learning Jan,16 2014  by @furufuru

1. Gathering image data for learning
ANPR program has a gathering image function.
If you make tmp/ directory on the execution environment, you can get number plate image.
And if make tmpChars/ directory on the execution environment, you ca get number image.

ex. 
  % mkdir tmp tmpChars
  % ../build/ANPR ../build/test/SDIM0366-800x533.jpg


2. Machine learning process

(1) Learning Number Plate image
After you move the gathering number plate images to two directory in order to divide true and false, you can execute machine lerning process for recognising Number Plate.

ex.
  % mkdir tmp/OK tmp/NG
  % (... move image to OK or NG directories ...)
  % ../build/trainSVM tmp/OK/ tmp/NG/
    (You can get 'SVM.xml'.)

(2) Learning Number image
After you move the gathering number images to ten directory (0 ... 9) in order to separate each numbers, you can execute machine lerning process for recognising Number (OCR).

ex.
 % (cd tmpChars; mkdir 0 1 2 3 4 5 6 7 8 9)
 % (... move image to 0 .. 9 directories ...)
 % ../build/trainOCR tmpChars/
   (You can get 'OCR.xml')


