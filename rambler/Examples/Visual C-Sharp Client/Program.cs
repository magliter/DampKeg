using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Rambler.XMPP.IM.Client;

namespace Visual_C_Sharp_Client
{
    class Program
    {
        static void Main(string[] args)
        {
            System.Console.WriteLine("Running");

            Client client = new Client("alpha@dampkeg.com");
            //
            client.start();

            System.Console.WriteLine("Press any key to continue...");
            System.Console.ReadKey();
        }
    }
}
