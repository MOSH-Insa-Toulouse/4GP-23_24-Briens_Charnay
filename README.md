# 4GP-23_24-Briens_Charnay

Projet réalisé sur l'année universitaire 2023/2024 de la formation 4GP, dans le cadre de l'Unité de Formation "du capteur au banc de test".
Le ci-présent fichier README donne tous les éléments de compréhension du projet global, ainsi que de toutes ses composantes. Cela est réalisé de sorte à permettre à l'utilisateur de reprendre tous les fichiers du présent dépôt GitHub, et de s'en servir.

## Sommaire

***
 - [Introduction](#introduction)
 - [Circuit Electronique Analogique](#circuit-electronique-analogique)
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

***
## Circuit Electronique Analogique
