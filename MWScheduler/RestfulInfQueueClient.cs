using System.IO;
using System.Net;
using System.Runtime.Remoting;
using System.Text;

namespace MWScheduler
{
    public class RestfulInfQueueClient<T> : IInfQueue<T>
        where T : class
    {
        public delegate void EventHandler(T obj);

        public event EventHandler OnPop;

        private readonly IPAddress m_Address;

        private readonly int m_Port;

        private RestfulInfQueueClient(IPAddress address, int port)
        {
            m_Address = address;
            m_Port = port;
        }

        public RestfulInfQueueClient(IPEndPoint endPoint) : this(endPoint.Address, endPoint.Port) { }

        public T Top => Request("GET");

        public T Lock() => Request("POST");

        public bool Pop(T obj)
        {
            OnPop?.Invoke(obj);
            return PopRequest(obj);
        }

        private bool PopRequest(T obj)
        {
            var req = WebRequest.CreateHttp($"http://{m_Address}:{m_Port}/pop");
            req.KeepAlive = false;
            req.Accept = "text/plain";
            req.ContentType = "text/xml";
            req.Method = "POST";

            using (var stream = req.GetRequestStream())
                RestfulSerialization<T>.Serialize(stream, obj);

            var res = req.GetResponse();
            using (var stream = res.GetResponseStream())
                if (stream != null)
                {
                    var buff = new byte[res.ContentLength];
                    stream.Read(buff, 0, (int)res.ContentLength);
                    var s = Encoding.UTF8.GetString(buff);
                    if (s == "ok")
                        return true;
                }
            return false;
        }

        private T Request(string method)
        {
            var req = WebRequest.CreateHttp($"http://{m_Address}:{m_Port}/");
            req.KeepAlive = false;
            req.Accept = "text/xml";
            req.Method = method;

            var res = req.GetResponse();
            using (var stream = res.GetResponseStream())
                if (stream != null)
                {
                    var buff = new byte[res.ContentLength];
                    stream.Read(buff, 0, (int)res.ContentLength);
                    return RestfulSerialization<T>.Deserialize(Encoding.UTF8.GetString(buff));
                }

            throw new RemotingException();
        }
    }
}
