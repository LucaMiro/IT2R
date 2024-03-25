#========================== Programme Base TP2 ==== HD 2024 ======================================


from fpioa_manager import fm
from Maix import GPIO

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

fm.register(15, fm.fpioa.GPIO0) # Attribue la pin IO15 à la fonction logicielle GPIO0
PIN_STOP = GPIO(GPIO.GPIO0, GPIO.OUT)

modele = kpu.load(0x300000)   # Chargement des coefficients du modèle depuis l'adresse 0x300000

anchors = (3.19,3.12,2.19,2.19,0.83,0.88,4.28,4.06,1.53,1.61)

kpu.init_yolo2(modele, 0.5, 0.3, 5, anchors) # Initialisation du calculateur IA



#-------- Boucle principale : Capture image -> Calcul IA -> Affichage résultats ---------------

while True:

        img = sensor.snapshot()  # Capture d'une image
        Resultats = kpu.run_yolo2(modele, img)  # Récupération du résultat du calculateur IA

        if Resultats :
            PIN_STOP.value(1)
            for i in Resultats :  # il peut y avoir plusieurs identifications
                img.draw_rectangle(i.rect(),color=(0,255,0)) # Tracé de la boite englobante
                print(i)    # Sur terminal série, affichage du contenu de chaque résultat

        lcd.display(img)

        PIN_STOP.value(0)



