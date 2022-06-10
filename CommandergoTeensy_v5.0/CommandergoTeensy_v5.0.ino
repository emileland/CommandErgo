/////////////////////////////////////////
//Command'ergo v5.0 Teensy             //
//Emile Landerretche                   //
//26/12/2021                           //
/////////////////////////////////////////




//initialise les bibliotheques
#include <NintendoExtensionCtrl.h>//bibliotheque pour l'utilisation de la nunchuk
#include<EEPROM.h>//ibliotheque pour la mémoire EEPROM (petit disque dur)

//initialisation des manettes nunchuk
Nunchuk nchuk1(Wire);//nunchuk 1
Nunchuk nchuk2(Wire1);//nunchuk 2

//Attribution des pins de branchements des boutons
const int pBtnPlus1 = 4;//pin du bouton plus n°1
const int pBtnMoins1 = 5;//pin du bouton moins n°1
const int pBtnPlus2 = 2;//pin du bouton plus n°2
const int pBtnMoins2 = 3;//pin du bouton moins n°2

//initialisation des variables
int touchesAssignations[21] = {};//variable qui stock les valeurs communiqués par l'application et les retransmet dans la mémoire EEPROM et au boitier qui fais la correspondance avec le tableau #coorespondant

boolean CChuk1 = 0;//permet de vérifier si la nunchuk 1 est connecté
boolean CChuk2 = 0;//permet de vérifier si la nunchuk 2 est connecté

int sensi;//variable stockant la sensibilté de la souris quand elle est utiliséq

int left;//variable qui stock la position du clic de souris gauche
int middle;//variable qui stock la position du clic de souris milieu
int right;//variable qui stock la position du clic de souris droite

const int correspondant[]//variable qui stock les valeurs qui correspondent a chaques touches et qui sont ensuite utilisés dans la fonction keyboard.press()
{
  "",
  61444,//keyA
  61445,//keyB
  61446,//keyC
  61447,//keyD
  61448,//keyE
  61449,//keyF
  61450,//keyG
  61451,//keyH
  61452,//keyI
  61453,//keyJ
  61454,//keyK
  61455,//keyL
  61456,//keyM
  61457,//keyN
  61458,//keyO
  61459,//keyP
  61460,//keyQ
  61461,//keyR
  61462,//keyS
  61463,//keyT
  61464,//keyU
  61465,//keyV
  61466,//keyW
  61467,//keyX
  61468,//keyY
  61469,//keyZ
  61479,//Key0
  61470,//key1
  61471,//key2
  61472,//key3
  61473,//key4
  61474,//key5
  61475,//key6
  61476,//key7
  61477,//key8
  61478,//key9
  61538,//keypad 0
  61529,//keypad 1
  61530,//keypad 2
  61531,//keypad 3
  61532,//keypad 4
  61533,//keypad 5
  61534,//keypad 6
  61535,//keypad 7
  61536,//keypad 8
  61537,//keypad 9
  61484,//espace
  61481,//echap
  61497,//verr maj
  61483,//tab
  61480,//entrée
  61516,//suppr
  61482,//backspace
  57345,//ctrl gauche
  57360,//ctrl droite
  57346,//maj gauche
  57376,//maj droite
  57348,//alt
  57408,//alt gr
  61522,//fleche haut
  61521,//fleche bas
  61520,//fleche gauche
  61519,//fleche droite
  1,//Clic gauche
  2,//"Clic droit
  3,//"Clic molette
  4,//"Sensi souris +
  5,//"Sensi souris -
  61498,//f1
  61499,//f2
  61500,//f3
  61501,//f4
  61502,//f5
  61503,//f6
  61504,//f7
  61505,//f8
  61506,//f9
  61507,//f10
  61508,//f11
  61509,//f12
};


void setup ()
{
  //attribut les pins de chaques boutons en mode input pullup : entrée avec une valeur "reset" haut
  pinMode(pBtnPlus1, INPUT_PULLUP);//bouton plus n°1
  pinMode(pBtnMoins1, INPUT_PULLUP);//bouton moins n°1
  pinMode(pBtnPlus2, INPUT_PULLUP);//bouton plus n°2
  pinMode(pBtnMoins2, INPUT_PULLUP);//bouton moins n°2

  for (int i = 0; i < 21; i++)//boucle répétée 21 fois
  {
    touchesAssignations[i] = EEPROM.read(i);//lis la combinaison de touches utilisés la derniere fois (stocker dans la mémoire EEPROM) et le stock dans la variable touchesAssignations
  }

  sensi = touchesAssignations[1];//met a jour la sensibilité

  Serial.begin(9600);//Initialise le port série avec un baud rate de 9600
  pinMode(LED_BUILTIN, OUTPUT);//Initialise la led d'état présente sur la carte teensy
  digitalWrite(LED_BUILTIN, HIGH);//Allume la led d'état

  //Initialisation des nunchuks
  nchuk1.begin();
  nchuk1.i2c().setClock(400000);
  nchuk2.begin();
  nchuk2.i2c().setClock(400000);

  //Essaye de connecter les nunchuks
  //nchuk1.connect();
  //nchuk2.connect();
  CChuk1 = nchuk1.connect();//stock l'état de connexion de la nunchuk 1 : true si connecté, false si non-connecté
  CChuk2 = nchuk2.connect();//stock l'état de connexion de la nunchuk 2 : true si connecté, false si non-connecté
}

//boucle principal
void loop ()
{
  digitalWrite(13, LOW);//éteint la led d'état
  if (CChuk1 == 1)//vérifie si la nunchuk 1 est connecté
  {
    nunchuk1();//si la nunchuk 1 est connecté lancer le programme pour la nunchuk 1
  }
  else//sinon si la nunchuk 1 n'est pas connecté
  {
    //alors retenter la connexion
    CChuk1 = nchuk1.connect();
  }
  if (CChuk2 == 1)//vérifie si la nunchuk 2 est connecté
  {
    nunchuk2();//si la nunchuk 2 est connecté lancer le programme pour la nunchuk 2
  }
  else//sinon si la nunchuk 2 n'est pas connecté
  {
    //alors retenter la connexion
    CChuk2 = nchuk2.connect();
  }


  communication();//lancer le programme de communication
}



void nunchuk1()//programme pour le premier joueur (nunchuk 1)
{
  nchuk1.update();//mise a jour des donnés de la nunchuk 1
  int vBtnPlus1 = digitalRead(pBtnPlus1);//créer une variable stockant l'état du bouton plus n°1
  int vBtnMoins1 = digitalRead(pBtnMoins1);// créer une variable stockant l'état du bouton moins n°1s
  int vBtnZ1 = nchuk1.buttonZ();//créer une variable stockant l'état du bouton Z de la nunchuk 1
  int vBtnC1 = nchuk1.buttonC();//créer une variable stockant l'état du bouton C de la nunchuk 1
  int vJoyX1 = nchuk1.joyX();//créer une variable stockant l'état de l'axe X de la nunchuk 1
  int vJoyY1 = nchuk1.joyY();//créer une variable stockant l'état de l'axe Y de la nunchuk 1
  vJoyX1 = map(vJoyX1, 32, 237, 0, 6);//change l'échelle des valeurs pour l'axe X de la nunchuk n°1 de 0 à 6 (c'est plus simple pour la suite)
  vJoyY1 = map(vJoyY1, 24, 222, 0, 6);//change l'échelle des valeurs pour l'axe Y de la nunchuk n°1 de 0 à 6 (c'est plus simple pour la suite)


  if (touchesAssignations[0] == 1)//vérifie si la case 0 du tableau touchesAssignations est égal à 1, si elle est bien égal à 1 alors cela veut dire qu'on utilise la souris
  {
    if (vJoyX1 <= 1)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(a gauche)
    {
      Mouse.move(-(sensi), 0);//deplacer la souris vers la gauche
    }


    if (vJoyX1 >= 5)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(a droite)
    {
      Mouse.move(sensi, 0);//deplacer la souris vers la droite
    }


    if (vJoyY1 <= 1)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(en bas)
    {
      Mouse.move(0, sensi);//deplacer la souris vers le bas
    }


    if (vJoyY1 >= 5)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(en haut)
    {
      Mouse.move(0, -(sensi));//deplacer la souris vers le haut
    }
  }
  else//sinon si la case 0 du tableau touchesAssignations n'est pas égal à 1, cela veut dire que lorsque le joystick bouge il faudra appuyer sur une touche de clavier
  { 
    if (vJoyX1 <= 1)//vérifier si le joystick est a gauche
    {
      appuyer(correspondant[touchesAssignations[8]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
    }
    else//si le joyqtick n'est pas a gauche
    {
      appuyer(correspondant[touchesAssignations[8]], 1);//si le joystick n'est pas a gauche alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }

    if (vJoyX1 >= 5)//vérifier si le joystick est a droite
    {
      appuyer(correspondant[touchesAssignations[9]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
      }
    else
    {
      appuyer(correspondant[touchesAssignations[9]], 1);//si le joystick n'est pas a droite alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }


    if (vJoyY1 <= 1)//vérifier si le joystick est en bas
    {
      appuyer(correspondant[touchesAssignations[7]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
    }
    else
    {
      appuyer(correspondant[touchesAssignations[7]], 1);//si le joystick n'est pas en bas alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }


    if (vJoyY1 >= 5)//vérifier si le joystick est en haut
    {
      appuyer(correspondant[touchesAssignations[6]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
    }
    else
    {
      appuyer(correspondant[touchesAssignations[6]], 1);//si le joystick n'est pas en haut alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }
  }

  if (vBtnZ1 == 1)//dans tout les cas vérifier si le bouton Z de la nuchuk est presser
  {
    appuyer(correspondant[touchesAssignations[4]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[4]], 1);//si le bouton Z n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }


  if (vBtnC1 == 1)//dans tout les cas vérifier si le bouton C de la nuchuk est presser
  {
    appuyer(correspondant[touchesAssignations[5]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[5]], 1);//si le bouton C n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }

  if (vBtnPlus1 == 0)//dans tout les cas vérifier si le bouton + est presser
  {
    appuyer(correspondant[touchesAssignations[10]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[10]], 1);//si le bouton + n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }

  if (vBtnMoins1 == 0)//dans tout les cas vérifier si le bouton - est presser
  {
    appuyer(correspondant[touchesAssignations[11]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[11]], 1);//si le bouton - n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }
}


void nunchuk2()//programme pour le deuxieme joueur (nunchuk 2)
{
  nchuk2.update();//mise a jour des donnés de la nunchuk 1
  int vBtnPlus2 = digitalRead(pBtnPlus2);//créer une variable stockant l'état du bouton plus n°1
  int vBtnMoins2 = digitalRead(pBtnMoins2);// créer une variable stockant l'état du bouton moins n°1s
  int vBtnZ2 = nchuk2.buttonZ();//créer une variable stockant l'état du bouton Z de la nunchuk 1
  int vBtnC2 = nchuk2.buttonC();//créer une variable stockant l'état du bouton C de la nunchuk 1
  int vJoyX2 = nchuk2.joyX();//créer une variable stockant l'état de l'axe X de la nunchuk 1
  int vJoyY2 = nchuk2.joyY();//créer une variable stockant l'état de l'axe Y de la nunchuk 1
  vJoyX2 = map(vJoyX2, 32, 237, 0, 6);//change l'échelle des valeurs pour l'axe X de la nunchuk n°1 de 0 à 6 (c'est plus simple pour la suite)
  vJoyY2 = map(vJoyY2, 24, 222, 0, 6);//change l'échelle des valeurs pour l'axe Y de la nunchuk n°1 de 0 à 6 (c'est plus simple pour la suite)


  if (touchesAssignations[2] == 1)//vérifie si la case 0 du tableau touchesAssignations est égal à 1, si elle est bien égal à 1 alors cela veut dire qu'on utilise la souris
  {
    if (vJoyX2 <= 1)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(a gauche)
    {
      Mouse.move(-(sensi), 0);//deplacer la souris vers la gauche
    }


    if (vJoyX2 >= 5)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(a droite)
    {
      Mouse.move(sensi, 0);//deplacer la souris vers la droite
    }


    if (vJoyY2 <= 1)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(en bas)
    {
      Mouse.move(0, sensi);//deplacer la souris vers le bas
    }


    if (vJoyY2 >= 5)//si la touche 0 de la touche assignation est égal à 1 alors vérifier la position du joystick(en haut)
    {
      Mouse.move(0, -(sensi));//deplacer la souris vers le haut
    }
  }
  else//sinon si la case 0 du tableau touchesAssignations n'est pas égal à 1, cela veut dire que lorsque le joystick bouge il faudra appuyer sur une touche de clavier
  { 
    if (vJoyX2 <= 1)//vérifier si le joystick est a gauche
    {
      appuyer(correspondant[touchesAssignations[16]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
    }
    else//si le joyqtick n'est pas a gauche
    {
      appuyer(correspondant[touchesAssignations[16]], 1);//si le joystick n'est pas a gauche alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }

    if (vJoyX2 >= 5)//vérifier si le joystick est a droite
    {
      appuyer(correspondant[touchesAssignations[17]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
      }
    else
    {
      appuyer(correspondant[touchesAssignations[17]], 1);//si le joystick n'est pas a droite alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }


    if (vJoyY2 <= 1)//vérifier si le joystick est en bas
    {
      appuyer(correspondant[touchesAssignations[15]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
    }
    else
    {
      appuyer(correspondant[touchesAssignations[15]], 1);//si le joystick n'est pas en bas alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }


    if (vJoyY2 >= 5)//vérifier si le joystick est en haut
    {
      appuyer(correspondant[touchesAssignations[14]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
    }
    else
    {
      appuyer(correspondant[touchesAssignations[14]], 1);//si le joystick n'est pas en haut alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
    }
  }

  if (vBtnZ2 == 1)//dans tout les cas vérifier si le bouton Z de la nuchuk est presser
  {
    appuyer(correspondant[touchesAssignations[12]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[12]], 1);//si le bouton Z n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }


  if (vBtnC2 == 1)//dans tout les cas vérifier si le bouton C de la nuchuk est presser
  {
    appuyer(correspondant[touchesAssignations[13]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[13]], 1);//si le bouton C n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }

  if (vBtnPlus2 == 0)//dans tout les cas vérifier si le bouton + est presser
  {
    appuyer(correspondant[touchesAssignations[18]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[18]], 1);//si le bouton + n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }

  if (vBtnMoins2 == 0)//dans tout les cas vérifier si le bouton - est presser
  {
    appuyer(correspondant[touchesAssignations[19]], 0);//alors dans ce cas appeler la fonction appuyer avec comme parametre 0 (pour appuyer sur la touche) et la case correspondante du tableau "correspondant" de la touche asssigner (cette ligne permet la conversion de la touche en hexadécimal)
  }
  else
  {
    appuyer(correspondant[touchesAssignations[19]], 1);//si le bouton - n'est pas presser alors appeler la fonction appuyer avec en parametre 0 pour relacher la touche correspondante
  }
}




//fonction qui permet la communication avec l'application
void communication()//declaration de la fonction
{
  if (Serial.available())//vérifie si le port série est utilisé
  {
    String lu = Serial.readString();//Si il est utilisé récupérer la chaine de caractère qu'il envoi
    createArray(lu);//Appeller la fonction qui créer un tableau en lui mettatnt la chaine de caractère recu comme argument

    //écrit dans l'EEPROM (mémoire disque dur de la carte) les noouvelles touches assignés pour pouvoir les récupérer au prochain démarrage
    for (int i = 0; i < 21; i++)//répeter 20fois
    {
      EEPROM.write(i, touchesAssignations[i]);//écrire la touche assignés dans la case correspondante de l'EEPROM
    }
    
    sensi = touchesAssignations[1];//initialiser la variable sensi a la sensibilité sélectionner dans l'application
  }

}

//fonction qui permet de convertir la chaine de caractère recu de l'application dans un tableau compréhensible
void createArray(String recu)//déclaration de la fonction, l'argument recu contient la chaine de caractere communiquer par l'application
{
  String touchesAssignationsString[21];//créer une variable temporaire qui stockera les caractères recu découper
  int compteur;//variable utiliser dans la prochaine boucle
  String chksumString;//variable qui stockera le checksum en version chaine de caractere, le checksum permet de vérifier si se qu'on a recu viens bien de l'application
  int chksum;//variable qui stockera le checksum en version nombre, le checksum permet de vérifier si se qu'on a recu viens bien de l'application

  //découper la chaine de caractère recu de l'application dans un tableau de caractere
  for (int i = 0; i < 21; i++)//répeter 20 fois
  {
    touchesAssignationsString[i] = recu[compteur];//la chaine de caratere est construite de facon que chaque parametre different fasse 2 caractere, cette ligne permet de récupérer le premier chiffre du parametre 
    touchesAssignationsString[i] = touchesAssignationsString[i] + recu[compteur + 1];//la chaine de caratere est construite de facon que chaque parametre different fasse 2 caractere, cette ligne permet de récupérer le deuxieme chiffre du parametre
    compteur = compteur + 2;// ici on augmente le compteur de 2 pour passer au parametre suivant (2 caractere plus loin dans la chaine)
  }
  compteur = 0;//reinitialisation du compteur

  //convertir chaque parametre de chaine de caractere à nombre
  for (int i = 0; i < 21; i++)//répeter 20 fois
  {
    touchesAssignations[i] = touchesAssignationsString[i].toInt();//transformer chaque valeur du tableau touichesAssignationsString en entier puis le stocker dans la variable final touchesAssignations sous forme de nombre
  }

  //calculer le checksum
  for (int i = 0; i < 20; i++)
  {
    chksum += touchesAssignations[i];
  }
  chksumString = String(chksum);
  int depart = chksumString.length() - 2;
  chksumString = chksumString.substring(depart);
  chksum = chksumString.toInt();
}


void appuyer(int i, int des)//i est sois un chiffre compris enrte zéro et 5 qui permet de gérer les clics de la souris sois au dessus de 4 est une valeur héxadécimal d'une touche ;;; des est une variable qui vérifie si l'on doit relacher une touche/clic ou la presser
{
  //sensi = 10;//variable stockant la sensibilité quand on utilise la souris
  bool sensiPlusOk;//booleen permettant d'éviter une boucle trop rapide quand on utilise la fonction de modification de la sensi
  bool sensiMoinsOk;//booleen permettant d'éviter une boucle trop rapide quand on utilise la fonction de modification de la sensi



  if (des == 0)//vérifie si des est a 0, si des est a zéro c'est le premier palier ou l'on dois appuyer sur la touche/clic
  {
    if (i == 1)//si i est égal a 1
    {
      left = 1;//mettre la variable left à 1
    }
    else if (i == 2)//si i est égal a 2
    {
      middle = 1;//mettre la variable middle à 1
    }
    else if (i == 3)//si i est égal a 3
    {
      right = 1;//mettre la variable right à 1
    }
    else if (i == 4 && sensiPlusOk == true)//vérifie si i est égal à 4 et si la variable sensiPlusOk est vrai (cela permet de pas augmenter d'un coup rapidement et créer des paliers)
    {
      sensi ++;//ajouter 1 a la sensibilité
      sensiPlusOk = false;//mettre la variable sensiPlusOk à faux (cela permet de pas augmenter d'un coup rapidement et créer des paliers)
    }
    else if (i == 5 && sensiMoinsOk == true)//vérifie si i est égal à 5 et si la variable sensiMoinsOk est vrai (cela permet de pas augmenter d'un coup rapidement et créer des paliers)
    {
      sensi --;//enlever 1 a la sensiblité
      sensiMoinsOk = false;//mettre la variable sensiMoinsOk à faux (cela permet de pas augmenter d'un coup rapidement et créer des paliers)
    }
    else// sinon cela veut dire que c'est un code hexadécimal et qu'il suffit de l'écrire
    {
      Keyboard.press(i);//écrire le code héxadécimal
    }
  }
  else if (des == 1)//vérifie si des est a 1, si des est a 1 c'est le deuxieme palier ou lon dois relacher la touche/clic
  {
    if (i == 1)//si i est égal a 1
    {
      left = 0;//mettre la variable left a 0
    }
    else if (i == 2)//si i est égal a 2
    {
      middle = 0;//mettre la variable middle a 0
    }
    else if (i == 3)//si i est égal a 3
    {
      right = 0;//mettre la variable right a 0      
    }
    else if (i == 4 && sensiPlusOk == false)//vérifie si i est égal à 4 et si la variable sensiPlusOk est faux (cela permet de pas augmenter d'un coup rapidement et créer des paliers)
    {
      sensiPlusOk = true;//dans ce cas mettre sensiMoinsOK a vrai pour redémarrer un cycle
    }
    else if (i == 5 && sensiMoinsOk == false)//vérifie si i est égal à 5 et si la variable sensiMoinsOk est faux (cela permet de pas augmenter d'un coup rapidement et créer des paliers)
    {
      sensiMoinsOk = true;//dans ce cas mettre sensiMoinsOK a vrai pour redémarrer un cycle
    }
    else// sinon cela veut dire que c'est un code hexadécimal et qu'il suffit de le relacher
    {
      Keyboard.release(i);//relacher  la touche correspondante
    }
  }
  Mouse.set_buttons(left, middle, right);//appuyer sur les clic de la souris en fonction des variables défini ci-dessus
}
