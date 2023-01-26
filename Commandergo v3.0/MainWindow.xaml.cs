/////////////////////////////////////////
//Command'ergo v5.0 Application        //
//Emile Landerretche                   //
//26/12/2021                           //
/////////////////////////////////////////


using System;
using System.IO;
using System.IO.Ports;
using System.Windows;
using Microsoft.Win32;


namespace Commandergo_v3._0
{
    public partial class MainWindow : Window
    {

        public int[] assignations = new int[17];//tableau stockant les valeurs assignés (que les touches sans les valeurs de la souris et du checksum)
        String[] stringTouches = new string[]//tableau de correspondance des caracteres en fonction de lindex de la cellule (ex: a:1 ; b:2 ; ...)
           {
                "",
                "a",
                "b",
                "c",
                "d",
                "e",
                "f",
                "g",
                "h",
                "i",
                "j",
                "k",
                "l",
                "m",
                "n",
                "o",
                "p",
                "q",
                "r",
                "s",
                "t",
                "u",
                "v",
                "w",
                "x",
                "y",
                "z",
                "key 0",
                "key 1",
                "key 2",
                "key 3",
                "key 4",
                "key 5",
                "key 6",
                "key 7",
                "key 8",
                "key 9",
                "keypad 0",
                "keypad 1",
                "keypad 2",
                "keypad 3",
                "keypad 4",
                "keypad 5",
                "keypad 6",
                "keypad 7",
                "keypad 8",
                "keypad 9",
                "espace",
                "echap",
                "verr maj",
                "tab",
                "entrée",
                "suppr",
                "backspace",
                "ctrl gauche",
                "ctrl droite",
                "maj gauche",
                "maj droite",
                "alt",
                "alt gr",
                "fleche haut",
                "fleche bas",
                "fleche gauche",
                "fleche droite",
                "Clic gauche",
                "Clic molette",
                "Clic droit",
                "Sensi souris +",
                "Sensi souris -",
                "f1",
                "f2",
                "f3",
                "f4",
                "f5",
                "f6",
                "f7",
                "f8",
                "f9",
                "f10",
                "f11",
                "f12"
           };
        public MainWindow()
        {
            InitializeComponent();
        }

        //lorsque la fenetre est ouverte initialiser les elements
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            InitListBox();
            souris1();
            refresh();
        }

        //

        private void Souris1_Click(object sender, RoutedEventArgs e)
        {
            souris1();
        }

        private void BTN_Z1_Click(object sender, RoutedEventArgs e)
        {
            assigner(1);
        }

        private void BTN_C1_Click(object sender, RoutedEventArgs e)
        {
            assigner(2);
        }

        private void BT_joystickHaut1_Click(object sender, RoutedEventArgs e)
        {
            assigner(3);
        }

        private void BT_joystickBas1_Click(object sender, RoutedEventArgs e)
        {
            assigner(4);
        }

        private void BT_joystickGauche1_Click(object sender, RoutedEventArgs e)
        {
            assigner(5);
        }

        private void BT_joystickDroite1_Click(object sender, RoutedEventArgs e)
        {
            assigner(6);
        }

        private void BTN_Plus1_Click(object sender, RoutedEventArgs e)
        {
            assigner(7);
        }

        private void BTN_Moins1_Click(object sender, RoutedEventArgs e)
        {
            assigner(8);
        }

        //activer ou non la souris 1
        private void souris1()
        {
            if (CHCKBOX_Souris1.IsChecked == false)
            {
                BT_joystickHaut1.IsEnabled = true;
                BT_joystickBas1.IsEnabled = true;
                BT_joystickGauche1.IsEnabled = true;
                BT_joystickDroite1.IsEnabled = true;
                sensiSouris1.IsEnabled = false;

                BT_joystickHaut1.Content = stringTouches[assignations[3]];
                BT_joystickBas1.Content = stringTouches[assignations[4]];
                BT_joystickGauche1.Content = stringTouches[assignations[5]];
                BT_joystickDroite1.Content = stringTouches[assignations[6]];
            }
            else
            {
                BT_joystickHaut1.IsEnabled = false;
                BT_joystickBas1.IsEnabled = false;
                BT_joystickGauche1.IsEnabled = false;
                BT_joystickDroite1.IsEnabled = false;
                sensiSouris1.IsEnabled = true;
                BT_joystickHaut1.Content = "";
                BT_joystickBas1.Content = "";
                BT_joystickGauche1.Content = "";
                BT_joystickDroite1.Content = "";
            }
        }



        //envoi au port COM selcetionner le tableau final (convertit en string avec la fonction CreateArray)
        public void BTN_Envoyer_Click(object sender, RoutedEventArgs e)
        {
            if (ready() == true)
            {
                SerialPort mySerialPort = new SerialPort();
                String selection = (string)ListeCOM.SelectedItem;
                mySerialPort.PortName = selection;

                try
                {
                    mySerialPort.Open();
                    string send = CreateArray();
                    mySerialPort.Write(send);
                    TXTBLCK_Log.Text = "envoi : " + send;
                    mySerialPort.Close();
                }
                catch (IOException ex)
                {
                    TXTBLCK_Log.Text = "Bug, Rééssayez "+ex+"!";
                }
            }







        }

        public void BTN_recuperer_Click(object sender, RoutedEventArgs e)
        {

        }

        //rafraichit les ports com disponibles
        private void BTN_Refresh_Click(object sender, RoutedEventArgs e)
        {
            refresh();
        }


    //lorsque un bouton est cliquer alrs asiigner les valeurs
        private void assigner(int quoi)
        {
            string stringSelection;
            int selection;


            if (listBox.SelectedItem != null)
            {
                //assigner la valeur selectionner dans le tableau final des touches
                selection = listBox.SelectedIndex;
                assignations[quoi] = selection;


                //montrer visuellement sur le bouton la touche assigner
                stringSelection = listBox.SelectedItem.ToString();
                switch (quoi)
                {
                    case 1:
                        BTN_Z1.Content = stringSelection;
                        break;
                    case 2:
                        BTN_C1.Content = stringSelection;
                        break;
                    case 3:
                        BT_joystickHaut1.Content = stringSelection;
                        break;
                    case 4:
                        BT_joystickBas1.Content = stringSelection;
                        break;
                    case 5:
                        BT_joystickGauche1.Content = stringSelection;
                        break;
                    case 6:
                        BT_joystickDroite1.Content = stringSelection;
                        break;
                    case 7:
                        BTN_Plus1.Content = stringSelection;
                        break;
                    case 8:
                        BTN_Moins1.Content = stringSelection;
                        break;
                    case 9:
                        BTN_Z2.Content = stringSelection;
                        break;
                    case 10:
                        BTN_C2.Content = stringSelection;
                        break;
                    case 11:
                        BT_joystickHaut2.Content = stringSelection;
                        break;
                    case 12:
                        BT_joystickBas2.Content = stringSelection;
                        break;
                    case 13:
                        BT_joystickGauche2.Content = stringSelection;
                        break;
                    case 14:
                        BT_joystickDroite2.Content = stringSelection;
                        break;
                    case 15:
                        BTN_Plus2.Content = stringSelection;
                        break;
                    case 16:
                        BTN_Moins2.Content = stringSelection;
                        break;
                }
            }
        }

        private void refresh()
        {
            String[] ports = new string[] { };
            ports = SerialPort.GetPortNames();

            int lenghtArray = ports.Length;

            for (int i = 0; i < lenghtArray; i++)
            {
                ListeCOM.Items.Clear();
                ListeCOM.Items.Add(ports[i]);
            }
        }

        private void InitListBox()
        {
            int lenghtArray = stringTouches.Length;

            for (int i = 0; i < lenghtArray; i++)
            {
                listBox.Items.Add(stringTouches[i]);
            }


            for (int i = 0; i < 11; i++)
            {
                ChooseSensi1.Items.Add(i);
                ChooseSensi1.SelectedIndex = 0;
            }
        }

        private bool ready()
        {
            if (ListeCOM.SelectedItem == null)
            {
                TXTBLCK_Log.Text = "Veuillez sélectionnez un port";
                return false;
            }
            else
            {
                return true;
            }
        }

        private string CreateArray()
        {
            string lesValeurs = "";
            decimal checksum = 0;
            // souris de la nchunk 1 ? 
            if (CHCKBOX_Souris1.IsChecked == true)
            {
                lesValeurs += "01";
                checksum += 1;
            }
            else
            {
                lesValeurs += "00";
            }
            // sensibilite de la souris 1
            lesValeurs += "0";
            lesValeurs += ChooseSensi1.SelectedItem;
            checksum += ChooseSensi1.SelectedIndex;
            // souris de la nchunk 2
            if (CHCKBOX_Souris2.IsChecked == true)
            {
                lesValeurs += "01";
                checksum += 1;
            }
            else
            {
                lesValeurs += "00";
            }

            lesValeurs += "00";

            // assignation des touches 1 a 16 
            for (int i = 1; i < 17; i++)
            {
                lesValeurs += assignations[i].ToString("00");
                checksum += assignations[i];
            }
            checksum = checksum - Math.Truncate(checksum / 100) * 100;
            lesValeurs += checksum.ToString("00");
            return lesValeurs;
        }

        private void inverseCreateArray(String recu)
        {
            String[] touchesAssignationsString = new string[21];
            int compteur = 0;      

            for (int i = 0; i < 21; i++) 
            {
                touchesAssignationsString[i] = recu[compteur].ToString();                                    
                touchesAssignationsString[i] = touchesAssignationsString[i] + recu[compteur + 1]; 
                compteur = compteur + 2;                                                          
            }
            compteur = 0; 


            BTN_Z1.Content = stringTouches[int.Parse(touchesAssignationsString[4])];
            BTN_C1.Content = stringTouches[int.Parse(touchesAssignationsString[5])]; ;
            BT_joystickHaut1.Content = stringTouches[int.Parse(touchesAssignationsString[6])]; ;
            BT_joystickBas1.Content = stringTouches[int.Parse(touchesAssignationsString[7])]; ;
            BT_joystickGauche1.Content = stringTouches[int.Parse(touchesAssignationsString[8])]; ;
            BT_joystickDroite1.Content = stringTouches[int.Parse(touchesAssignationsString[9])]; ;
            BTN_Plus1.Content = stringTouches[int.Parse(touchesAssignationsString[10])]; ;
            BTN_Moins1.Content = stringTouches[int.Parse(touchesAssignationsString[11])]; ;
            BTN_Z2.Content = stringTouches[int.Parse(touchesAssignationsString[12])]; ;
            BTN_C2.Content = stringTouches[int.Parse(touchesAssignationsString[13])]; ;
            BT_joystickHaut2.Content = stringTouches[int.Parse(touchesAssignationsString[14])]; ;
            BT_joystickBas2.Content = stringTouches[int.Parse(touchesAssignationsString[15])]; ;
            BT_joystickGauche2.Content = stringTouches[int.Parse(touchesAssignationsString[16])]; ;
            BT_joystickDroite2.Content = stringTouches[int.Parse(touchesAssignationsString[17])]; ;
            BTN_Plus2.Content = stringTouches[int.Parse(touchesAssignationsString[18])]; ;
            BTN_Moins2.Content = stringTouches[int.Parse(touchesAssignationsString[19])]; ;

            if (int.Parse(touchesAssignationsString[0]) == 1)
            {
                CHCKBOX_Souris1.IsChecked = true;
                BT_joystickHaut1.IsEnabled = false;
                BT_joystickBas1.IsEnabled = false;
                BT_joystickGauche1.IsEnabled = false;
                BT_joystickDroite1.IsEnabled = false;
                sensiSouris1.IsEnabled = true;
                BT_joystickHaut1.Content = "";
                BT_joystickBas1.Content = "";
                BT_joystickGauche1.Content = "";
                BT_joystickDroite1.Content = "";
            }
            else
            {
                CHCKBOX_Souris1.IsChecked = false;
                BT_joystickHaut1.IsEnabled = true;
                BT_joystickBas1.IsEnabled = true;
                BT_joystickGauche1.IsEnabled = true;
                BT_joystickDroite1.IsEnabled = true;
                sensiSouris1.IsEnabled = false;
            }

            if (int.Parse(touchesAssignationsString[2]) == 1)
            {
                CHCKBOX_Souris2.IsChecked = true;
                BT_joystickHaut2.IsEnabled = false;
                BT_joystickBas2.IsEnabled = false;
                BT_joystickGauche2.IsEnabled = false;
                BT_joystickDroite2.IsEnabled = false;
                BT_joystickHaut2.Content = "";
                BT_joystickBas2.Content = "";
                BT_joystickGauche2.Content = "";
                BT_joystickDroite2.Content = "";

            }
            else
            {
                CHCKBOX_Souris2.IsChecked = false;
                BT_joystickHaut2.IsEnabled = true;
                BT_joystickBas2.IsEnabled = true;
                BT_joystickGauche2.IsEnabled = true;
                BT_joystickDroite2.IsEnabled = true;
            }

            ChooseSensi1.SelectedIndex = int.Parse(touchesAssignationsString[1]);

            for (int i = 1; i < 17; i++)
            {
                assignations[i] = int.Parse(touchesAssignationsString[i + 3]);
            }
        }


        private void BTN_Z2_Click(object sender, RoutedEventArgs e)
        {
            assigner(9);
        }

        private void BTN_C2_Click(object sender, RoutedEventArgs e)
        {
            assigner(10);
        }

        private void BT_joystickHaut2_Click(object sender, RoutedEventArgs e)
        {
            assigner(11);
        }

        private void BT_joystickBas2_Click(object sender, RoutedEventArgs e)
        {
            assigner(12);
        }

        private void BT_joystickGauche2_Click(object sender, RoutedEventArgs e)
        {
            assigner(13);
        }

        private void BT_joystickDroite2_Click(object sender, RoutedEventArgs e)
        {
            assigner(14);
        }

        private void BTN_Plus2_Click(object sender, RoutedEventArgs e)
        {
            assigner(15);
        }

        private void BTN_Moins2_Click(object sender, RoutedEventArgs e)
        {
            assigner(16);
        }

        private void CHCKBOX_Souris2_Click(object sender, RoutedEventArgs e)
        {
            souris2();
        }


        private void souris2()
        {
            if (CHCKBOX_Souris2.IsChecked == false)
            {
                BT_joystickHaut2.IsEnabled = true;
                BT_joystickBas2.IsEnabled = true;
                BT_joystickGauche2.IsEnabled = true;
                BT_joystickDroite2.IsEnabled = true;

                BT_joystickHaut2.Content = stringTouches[assignations[3]];
                BT_joystickBas2.Content = stringTouches[assignations[4]];
                BT_joystickGauche2.Content = stringTouches[assignations[5]];
                BT_joystickDroite1.Content = stringTouches[assignations[6]];
            }
            else
            {
                BT_joystickHaut2.IsEnabled = false;
                BT_joystickBas2.IsEnabled = false;
                BT_joystickGauche2.IsEnabled = false;
                BT_joystickDroite2.IsEnabled = false;
                BT_joystickHaut2.Content = "";
                BT_joystickBas2.Content = "";
                BT_joystickGauche2.Content = "";
                BT_joystickDroite2.Content = "";
            }
        }

        private void BT_search_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == true)
                txtEditor.Text = Path.GetFullPath(openFileDialog.FileName);
        }



        private void BT_test_Click(object sender, RoutedEventArgs e)
        {
            inverseCreateArray("000500010105261917042324091615121113021413");
        }

        private void BT_Charger_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                String line;
                StreamReader sr = new StreamReader(txtEditor.Text);
                line = sr.ReadLine();
                sr.Close();

                inverseCreateArray(line);
                TXTBLCK_Log.Text = "fichier chargé !";
            }
            catch (Exception f)
            {
                TXTBLCK_Log.Text = "Exception: " + f.Message;
            }
        }

        private void BT_Save_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (MessageBox.Show("voulez vous vraiment sauvegarder cette configuration, si une ancienne etait deja present dans le fichier elle sera ecraser",
                    "Save file",
                    MessageBoxButton.YesNo,
                    MessageBoxImage.Question) == MessageBoxResult.Yes)
                {
                    StreamWriter sw = new StreamWriter(txtEditor.Text);
                    sw.WriteLine(CreateArray());
                    sw.WriteLine("Commandergo File");
                    //Close the file
                    sw.Close();
                    TXTBLCK_Log.Text = "Sauvegarder !";
                }
                else
                {
                    TXTBLCK_Log.Text = "Annuler !";
                }
            }
            catch (Exception g)
            {
                TXTBLCK_Log.Text = "Exception: " + g.Message;
            }
        }
    }
}

