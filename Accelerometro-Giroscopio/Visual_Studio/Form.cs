using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.IO;
namespace WindowsFormsApplication1
{
    public partial class FrmSimulador : Form
    {
        string datum= "";
        public FrmSimulador()
        {
            InitializeComponent();
            
            Control.CheckForIllegalCrossThreadCalls = false;
            foreach (string s in SerialPort.GetPortNames())
            {
                cBchoise.Items.Add(s);
            }
        }
        private void Form1_Load(object sender, EventArgs e) 
        {
            timer1.Start();
        }
       

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
                serialPort1.Write(txtWrite.ToString());
            else
                MessageBox.Show("No se ha abierto el puerto");
        }
 	/*select adn configured comunicattion serial*/
        private void cBchoise_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort1.PortName = cBchoise.Text;
            try
            {
                serialPort1.BaudRate = 9600;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.StopBits = StopBits.One;
                serialPort1.Handshake = Handshake.None;

                serialPort1.Open();
                if(serialPort1.IsOpen)
                {
                    timer1.Enabled = true;
                }
            }
            catch
            {
                MessageBox.Show("Puerto no valido");
                return;
            }
            cBchoise.Enabled = false;
        }
        Boolean i = false;
	  /*recive data from serial and send to the graph*/
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //string dato_recivido = serialPort1.ReadExisting().ToString();                      
            string dato_recivido = serialPort1.ReadLine().ToString();

            if (i == false)
            {
                rt = 0;
                i = true;
           } 
            IList<string> valores  = dato_recivido.Split(' ');
            datum = datum + dato_recivido;
            richTxtRead2.Text = datum;

            leds(valores);

            txtRead.Text = dato_recivido.ToString();
            GraficaXY.Series[0].Points.AddY(Convert.ToDouble(valores[2])); 
            GraficaXY.Series[1].Points.AddXY(rt, Convert.ToDouble(valores[4]));
            //lblSeconds1.Text = rt.ToString();
            StreamWriter writer = File.AppendText("C:\\Users\\JAR\\Desktop\\Test.txt");

         
            //writer.WriteLine(dato_recivido+"  ");
            writer.WriteLine(valores[0] + valores[2]+valores[4]); 
            writer.Close();
            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
        }
        private void leds(IList<string> values) 
        {
            if (values[0].Contains("OK"))
            {
                ovCenter.BackColor = System.Drawing.Color.Green;
                ovDown.BackColor = System.Drawing.SystemColors.ActiveBorder;
                ovIzq.BackColor = System.Drawing.SystemColors.ActiveBorder;
                ovDerecha.BackColor = System.Drawing.SystemColors.ActiveBorder;
                ovUp.BackColor = System.Drawing.SystemColors.ActiveBorder;
            }
            else if (values[0].Contains("IZQUIERDA"))
            {
                ovIzq.BackColor = System.Drawing.Color.Red;
                ovCenter.BackColor = System.Drawing.Color.Red;
                //ovDown.BackColor = System.Drawing.Color.Green;
            }
            else if (values[0].Contains("DERECHA"))
            {
                ovDerecha.BackColor = System.Drawing.Color.Red;
                ovCenter.BackColor = System.Drawing.Color.Red;
                //ovDown.BackColor = System.Drawing.Color.Green;
            }
            else if (values[0].Contains("ARRIBA"))
            {
                ovUp.BackColor = System.Drawing.Color.Red;
                ovCenter.BackColor = System.Drawing.Color.Red;
                //ovDown.BackColor = System.Drawing.Color.Green;
            }
            else if (values[0].Contains("ABAJO"))
            {
                ovDown.BackColor = System.Drawing.Color.Red;
                ovCenter.BackColor = System.Drawing.Color.Red;
                //ovDown.BackColor = System.Drawing.Color.Green;
            }
        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            serialPort1.Close();
        }
         double rt = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
              rt++;
            lblSeconds1.Text = rt.ToString();
        }

        private void btnTimerStop_Click(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            serialPort1.Close();
            GraficaXY.BackSecondaryColor = System.Drawing.Color.Coral;
            GraficaXY.Printing.PrintPreview();        
            GraficaXY.Printing.Print(true);
           
        }

    }
}
