#========================== Programme IA Projet IT2R ==== HD 2024 ======================================


from fpioa_manager import fm
from Maix import GPIO
from machine import UART

import time
import sensor, image, lcd
import KPU as kpu



#---------- Initialisation LCD et Caméra -----------
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.run(1)
sensor.set_windowing((224,224))



#---------- Initialisation KPU avec le modèle IA entrainé au préalable -------------------------

fm.register(12, fm.fpioa.GPIO0) # Attribue la pin IO15 à la fonction logicielle GPIO0

#----------------------------------------- UART1 -----------------------------------------------

fm.register(5, fm.fpioa.UART1_TX, force=True)
fm.register(4, fm.fpioa.UART1_RX, force=True)

#----------------------------------------- SPI to ESP32 ----------------------------------------

fm.register(28,fm.fpioa.SPI1_D0, force=True)#mosi
fm.register(26,fm.fpioa.SPI1_D1, force=True)#miso
fm.register(27,fm.fpioa.SPI1_SCLK, force=True)#sclk

PIN_STOP = GPIO(GPIO.GPIO0, GPIO.OUT)

modele = kpu.load(0x300000)   # Chargement des coefficients du modèle depuis l'adresse 0x300000

anchors = (3.19,3.12,2.19,2.19,0.83,0.88,4.28,4.06,1.53,1.61)

kpu.init_yolo2(modele, 0.5, 0.3, 5, anchors) # Initialisation du calculateur IA

uart = UART(UART.UART1, 115200, 8, 0, 1, timeout=2000, read_buf_len=4)


#-------- Boucle principale : Capture image -> Calcul IA -> Affichage résultats ---------------

while True:

        img = sensor.snapshot()  # Capture d'une image
        Resultats = kpu.run_yolo2(modele, img)  # Récupération du résultat du calculateur IA




        if Resultats :

            uart.write('S')
            time.sleep_ms(1)
            uart.write('T')
            time.sleep_ms(1)
            uart.write('O')
            time.sleep_ms(1)
            uart.write('P')
            time.sleep_ms(1)

            for i in Resultats :  # il peut y avoir plusieurs identifications
                img.draw_rectangle(i.rect(),color=(0,255,0)) # Tracé de la boite englobante
                print(i)    # Sur terminal série, affichage du contenu de chaque résultat




        lcd.display(img)





