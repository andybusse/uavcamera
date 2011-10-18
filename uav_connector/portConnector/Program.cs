using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Configuration;
using System.Net.Sockets;
using System.Collections.Specialized;
using System.Collections;
using System.Threading;

namespace portConnector
{
    class Program
    {
        static void Main(string[] args)
        {
            byte[] data = new byte[256];
            Connector connector1 = new Connector();
            connector1.ConnectToPort();
            data = connector1.dataFromUAV();
        }
    }
}
