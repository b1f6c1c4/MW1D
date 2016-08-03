using System;
using System.IO;
using System.Net;
using System.Text;
using MWLiteService;

namespace MWScheduler
{
    public class RestfulInfQueueServer<T>
        where T : class
    {
        public delegate void EventHandler(T obj);

        public event EventHandler OnPop;

        private readonly IInfQueue<T> m_BaseQueue;

        // ReSharper disable once PrivateFieldCanBeConvertedToLocalVariable
        private readonly HttpServer m_HttpServer;

        public RestfulInfQueueServer(IInfQueue<T> baseQueue, int port)
        {
            m_BaseQueue = baseQueue;
            m_HttpServer = new HttpServer(IPAddress.Any, port);
            m_HttpServer.OnHttpRequest += HttpServer_OnHttpRequest;
        }

        public void Start() => m_HttpServer.Start();

        private HttpResponse HttpServer_OnHttpRequest(HttpRequest request)
        {
            switch (request.Method)
            {
                case "GET":
                    if (request.BaseUri != "/")
                        throw new HttpException(404);
                    return Serialize(m_BaseQueue.Top);
                case "POST":
                    switch (request.BaseUri)
                    {
                        case "/":
                            return Serialize(m_BaseQueue.Lock());
                        case "/pop":
                            var lng = Convert.ToInt32(request.Header["Content-Length"]);
                            var buff = new byte[lng];
                            request.RequestStream.Read(buff, 0, lng);
                            var obj = RestfulSerialization<T>.Deserialize(Encoding.UTF8.GetString(buff));
                            OnPop?.Invoke(obj);
                            var res = m_BaseQueue.Pop(obj);
                            return HttpServer.GenerateHttpResponse(res ? "ok" : "ignored", "text/plain");
                        default:
                            throw new HttpException(404);
                    }
                default:
                    throw new HttpException(405);
            }
        }

        private static HttpResponse Serialize(T obj)
        {
            var stream = new MemoryStream();
            RestfulSerialization<T>.Serialize(stream, obj);
            return HttpServer.GenerateHttpResponse(stream, "text/xml");
        }
    }
}
