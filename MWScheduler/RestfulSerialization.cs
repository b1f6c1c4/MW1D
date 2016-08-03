using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace MWScheduler
{
    internal static class RestfulSerialization<T>
    {
        internal static void Serialize(Stream stream, T obj) => Serialize(new StreamWriter(stream), obj);

        private static void Serialize(TextWriter sw, T obj)
        {
            var xml = new XmlSerializer(typeof(T));
            xml.Serialize(sw, obj);
            sw.Flush();
        }

        internal static T Deserialize(string str) => Deserialize(new StringReader(str));

        private static T Deserialize(TextReader sr)
        {
            var xml = new XmlSerializer(typeof(T));
            return (T)xml.Deserialize(sr);
        }
    }
}
