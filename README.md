# duobot

## Introduction

Ce projet est un n-ième clone du célèbre "self balancing robot" que l'on peut voir un peut partout sur Internet.

Voici un lien vers le projet original: https://electricdiylab.com/diy-self-balancing-robot/

Pour ce projet, nous conservons les fondamentaux de l'original (moteur, drivers, accéléromètre, ...).

Nous utiliserons les matériaux et les outils à notre à notre disposition. Les dimensions du robot seront également les nôtres.

Et la carte sera développée dans le but d'être utilisée pour d'autres applications similaires et pour permettre la mise au point d'autres projets à base de moteurs pas-à-pas.

## Construction du robot


## Conception de la carte

### Schéma

Presque un copier-coller du modèle d'origine.

Les différences principales sont les suivantes:
- Pull up sur le Enable plutôt que des pull-down sur le Step de chaque driver;
- Branchement des M0, M1 et M2 sur l'arduino (pour permettre le réglage de la résolution);
- Possibilité de brancher soit un drv4988 (comme l'original), soit un drv8825 (broche VDD ou Fault que l'on peut connecter grâce à des résistances 0 Ohms);
- Ajout de condensateurs de découplage (comme préconisé dans la datasheet des drivers);
- Sortie des broches non utilisées sur un connecteur d'extension.

### Routage du PCB

Complètement revu pour une carte moins dense (un peu plus grande) de 100mm x 70mm.
Les connecteurs seront des borniers plutôt que des JST comme sur le modèle. Ca permet un peu plus de souplesse lors des essais...

### Vue en 3D

## Mise en service

### Réglage du courant MAX

On tourne le potentiomètre des drivers jusqu'à obtenir 1V. Cela correspond à 250mA environ. Ce réglage sera à afiner en fonction des performances et de la limite admissible par le moteur...

### Programme de test de la carte

Le programme permet de tester la carte.

- Il affiche les valeurs lues sur l'accéléromètre
- il permet d'actionner les moteurs à l'aide des commandes.

