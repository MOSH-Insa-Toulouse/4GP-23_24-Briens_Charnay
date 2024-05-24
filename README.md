# 4GP-23_24-Briens_Charnay

Projet réalisé sur l'année universitaire 2023/2024 de la formation 4GP, dans le cadre de l'Unité de Formation (UF) "du capteur au banc de test".
Le ci-présent fichier README donne tous les éléments de compréhension du projet global, ainsi que de toutes ses composantes. Cela est réalisé de sorte à permettre à l'utilisateur de reprendre tous les fichiers du présent dépôt GitHub, et de s'en servir.

## Sommaire

 - [Introduction](#introduction)
 - [Capteur à base de crayon graphite](#capteur-graphite)
 - [Circuit Electronique Analogique](#circuit-electronique-analogique)
 - [Kicad : réalisation du shield électronique PCB](#kicad)
 - [Banc de test](#banc-de-test)
 - [Interface Homme-Machine](#ihm)
 - [Programmation](#programmation)
 - [Datasheet](#datasheet)
***
## Introduction
### Contexte du projet

Nos travaux se sont basés sur les résultats d'une publication scientifique \[Cheng-Wei Lin et al., 2014]. Celle-ci étudiait l'utilisation de pistes de graphites sur des feuilles de papier comme capteurs de déformation, et donc de contrainte. Une telle technologie aurait de nombreux avantages :
 - Elle est sobre ;
 - Elle consomme peu d'énergie ;
 - Elle est simple d'utilisation ;
 - Elle est facilement réparable et remplaçable avec peu de ressources ;
 - Les ressources nécessaires sont disponibles au grand public pour un prix très réduit.

Du point de vue physique, la technologie se base sur les effets granulaires des couches de graphite déposées sur le papier : qui agissent comme des îlots conducteurs, séparés par une absence de graphite - non conductrice. Cela induit d'infimes capacités en très grand nombre. Lorsque le papier est déformé, la distance entre ces îlots varie, les capacités également. Cela se mesure sur la résistance du dispositif : dont l'évolution angulaire absolue varie linéairement.

### Contenu du projet

Comme annoncé par le nom de l'UF, notre projet a inclut toutes les étapes allant du capteur au bac de test, le tout en open source hardware. Les différentes phases clés du projet ont donc été, telles qu'annoncées en sommaire :
- le développement du capteur à jauge de contrainte à base de crayon graphite ;
- la conception d'un circuit analogique pour interfacer le capteur à jauge de contrainte;
- la programmation arduino de microcontrôleurs ;
- la réalisation d'une application android ;
- la réalisation d'un shield électronique PCB dédié à l'interface du capteur de contrainte ;
- la réalisation d'un banc de test.

Ces différentes étapes ne seront pas présentées dans un ordre temporel mais logique : de sorte à permettre pour chacune de comprendre l'intégralité des éléments.
***
## Capteur graphite
Le capteur graphite développé se présente sous la forme suivante, toutes les grandeurs sont données en mm :

![image](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030421/8eeb0cc3-0171-4c76-8e89-9ef92a2159c1)

On note le signal prévu pour chacun des pins :
- pin 1 : +5V
- pin 2 : sortie analogique
Les deux pins sont inversibles.

Les zones pointillées doivent être recouverte d'une épaisse couche de graphite, déposée par crayon.
Les pins utilisés sont supposés classiques, comme par exemple des pinces métalliques.
***
## Circuit Electronique Analogique



## Kicad
Logiciel utilisé pour la réalisation du shield électronique PCB



### Schematic



***![Kicad_schematic](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030278/80586963-72e6-4f57-b002-da07260c54e4)

### PCB

![Kicad_pcb](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030278/19ffef8f-08a0-4ab1-8dbb-274e28ce296c)


***
## Banc de test

***
## IHM
L'Interface Homme-Machine (ou IHM) correspond à tous les éléments permettant la communication entre l'utilisateur et les composants software.
### Boutons et écran OLED
Trois boutons ainsi qu'un écran OLED sont directement présents sur le shield :

![image](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030421/340486e6-ebd4-4215-90b5-d6d2bee74fce)

Du haut vers le bas, les trois boutons correspondent aux actions :
- select ;
- up ;
- down.

L'écran OLED affiche un total de 4 champs. Les deux premiers sont modifiables, les deux derniers correspondent à des mesures réalisées.
- la valeur du potentiomètre digital ;
- l'unité de mesure ;
- la valeur mesurée dans cette unité par le capteur graphite ;
- la valeur mesurée dans cette unité par le Flex Sensor, capteur commercial.

L'utilisateur contrôle un curseur, celui-ci a deux états disponibles : "- xyz -" et "< xyz >". Le premier correspond au choix du champ à modifier : il permet à l'utilisateur de choisir s'il souhait modifier la valeur du potentiomètre ou l'unité de mesure. Le deuxième correspond à la modification du champ actuel : les boutons up et down permettent alors de se déplacer parmis les différentes options proposées pour le champ actuel. La pression du bouton select permet de passer d'un état à l'autre.
### Application Android
Il est également possible de communiquer avec l'arduino via une application android connectée en bluetooth. Celle-ci se présente ainsi :

![image](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030421/e76200a8-36f0-4bc7-ae33-585483451dd8)

Chaque ligne possède un rôle :
- la première, "Bluetooth et fichier" : permet de se connecter à l'arduino en bluetooth, de s'en déconnecter, et d'exporter les données du graphe. L'export se fait dans un fichier texte généré dans le téléphone ;
- la deuxième, "Contrôle d'acquisition" : permet de lancer l'acquisition de données, de la stopper, ou de lancer un test automatisé de la jauge (voir banc de test) ;
- la troisième est le graphe sur lequel apparaissent les données récoltées ;
- la quatrième, "Contrôle graphique" : permet de remettre le graphique à zéro, et de changer l'unité de mesure en volts, en ohms, ou en degrés ;
- la cinquième, "Nombre de points" : permet de choisir le nombre de point maximum à enregistrer et afficher sur le graphique.

*Fichiers associés* :
- application android/AppMOSH1.apk
- application android/CodeAppAndroid.aia
***
## Programmation
### Protocoles de communication
Au sein de notre projet, nous avons mis en place deux protocoles de communication : entre le téléphone et l'arduino du capteur graphite (bluetooth), ainsi qu'entre cette dernière et l'arduino du banc de test (électronique directe).
#### Communication Bluetooth
Dans le cadre de la communication bluetooth entre le téléphone et l'arduino du capteur graphite, les données sont transmises sous format ASCII, par des messages de 16 caractères, sous le format suivent :

![image](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030421/0c7ee19f-f2b3-475a-8106-f0ee02a2e048)

Le téléphone est maître de la communication, l'arduino joue le rôle d'esclave. Le caractère "commande" permet de transmettre une information ou ordre spécifique, nous notons les différentes possibilités :

Du téléphone vers l'arduino :
- d : demander l'envoie d'un message ;
- V : choix d'une mesure en volts ;
- O : choix d'une mesure en ohms ;
- D : choix d'une mesure en degrés ;
- t : lancement d'un test automatisé.

De l'arduino vers le téléphone :
- V : message contenant une donnée en volts ;
- O : message contenant une donnée en ohms ;
- D : message contenant une donnée en degrés ;
- c : information du lancement d'un test automatisé ;
- f : information de la fin d'un test automatisé.
#### Communication électronique directe
Dans le cadre de la communication électronique directe entre les arduinos du capteur graphite et du banc de test, deux lignes de communication sont utilisées : chacune contrôlée par une carte arduino. Ce protocole de communication permet de synchroniser le lancement des tests automatisés entre le banc de test, et l'enregistrement des mesures sur le téléphone. Le chronogramme se présente ainsi :

![image](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030421/ad95b7a4-a301-4c27-9aa7-8dac9b24df70)

Nous détaillons chaque évolution :
- état initial : les deux lignes sont à l'état down ;
- l'arduino du capteur reçoit l'ordre de l'application de lancer le test automatisé, sa ligne passe à up ;
- l'arduino du banc de test lit l'information, lance le test, et passe sa ligne à up ;
- l'arduino du capteur lit que le test a bien été lancé, passe sa ligne à down, et envoie l'information à l'application ;
- une fois le test terminé, l'arduino du banc de test passe sa ligne à down ;
- l'arduino du capteur lit que le test est terminé, et envoie l'information à l'application.
### Programme Arduino
Deux programmes arduinos ont été créés, pour les deux cartes utilisées dans ce projet : celle du shield PCB (connectée au capteur graphite), et celle du banc de test. Nous détaillons leur fonctionnement.
#### Arduino du capteur graphite
Le fonctionnement général de l'arduino du capteur graphite suit la logique présentée dans le schéma suivant :

![image](https://github.com/MOSH-Insa-Toulouse/4GP-23_24-Briens_Charnay/assets/160030421/c279b4c9-5c15-4a89-a0fd-9067a9cf1cb4)

Le choix a été fait de réaliser un debouncing des boutons entièrement software.

*Fichier associé* : programme_complet/programme_complet.ino
#### Arduino du banc de test
L'arduino du banc de test possède un programme bien plus simple et direct. Celui-ci suit la structure suivante :
- ajuster la position du Servomoteur : à 0 si aucun test n'est en cours, sinon une autre valeur variable ;
- lire et écrire sur les lignes de communication avec l'arduino du capteur graphite.

*Fichier associé* : programme_second_arduino/prog_second_arduino/prog_second_arduino.ino
***
## Datasheet
Finalement, la combinaison de tous ces éléments a permis la récolte de données, et la rédaction d'une datasheet complète du capteur graphite. Celle-ci est divisée en plusieurs catégories :
- Caractéristiques générales : points clés à l'avantage du capteur ;
- Description : description de la physique du fonctionnement du capteur, développement de ses avantages clés, objectif du développement du capteur ;
- Description des pins : description des pins et signaux du capteur, permettant son implémentation électronique ;
- Dimensions : description matérielle du capteur ;
- Spécifications : description technique concise du capteur ;
- Conditions d'utilisation standard : conditions environnementales classiques, pour lesquelles le capteur a été développé ;
- Caractéristiques électriques : pour les différentes variantes du capteur (en fonction du crayon utilisé), évolution de la résistance absolue et relative en fonction de l'angle ;
- Utilisation typique : exemple d'un circuit électronique typique dans lequel implémenter le capteur pour en tirer des données.
Ce circuit électronique typique correspond à celui que nous avons réalisé sur l'arduino du capteur, et présenté précédemment.

Cette datasheet permet à tout nouvel arrivant de produire et d'utiliser notre capteur graphite : dans un esprit Low-Tech et Open Source.

*Fichier associé* : Datasheet_strain_gauge-MCharnay-ABriens.pdf
