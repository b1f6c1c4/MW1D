using System;
using System.Globalization;
using System.IO;
using System.Net;

namespace MWScheduler
{
    internal static class Program
    {
        private static void Main(string[] args)
        {
            const string theBaseDir = @"db/";
            const int defaultPort = 24680;

            Core core = new RationalCore();

            if (args.Length == 0) // stand alone
            {
                const string baseDir = theBaseDir;
                PrepareDir(baseDir);
                var root = ConstructRoot();
                var cached = ConstructCache(root, baseDir);
                Run(cached, core, baseDir);
                return;
            }

            if (args.Length == 1 &&
                args[0] == "--server") // stand alone + server
            {
                const string baseDir = theBaseDir;
                PrepareDir(baseDir);
                var root = ConstructRoot();
                var cached = ConstructCache(root, baseDir);
                var server = new RestfulInfQueueServer<Config>(cached, defaultPort);
                server.OnPop += cfg => DownloadResult(cfg, baseDir);
                server.Start();
                Run(cached, core, baseDir);
                return;
            }

            if (args.Length == 2 &&
                args[0] == "--server" &&
                args[1] == "--bare") // server
            {
                const string baseDir = theBaseDir;
                PrepareDir(baseDir);
                var root = ConstructRoot();
                var cached = ConstructCache(root, baseDir);
                var server = new RestfulInfQueueServer<Config>(cached, defaultPort);
                server.OnPop += cfg => DownloadResult(cfg, baseDir);
                server.Start();
                Console.Read();
                return;
            }

            if (args.Length == 2 &&
                args[0] == "--client") // client
            {
                const string baseDir = "";
                PrepareDir(baseDir);
                var remote = new RestfulInfQueueClient<Config>(ParseIPEndPoint(args[1], defaultPort));
                remote.OnPop += cfg => UploadResult(cfg, baseDir);
                var syncronized = SynInfQueue<Config>.Syncronize(remote);
                Run(syncronized, core, baseDir);
                return;
            }

            Console.WriteLine(@"Usage: MWScheduler [--client HOST:PORT | --server [--bare]]");
        }

        private static void DownloadResult(Config cfg, string baseDir)
        {
            var filePath = Path.Combine(baseDir, cfg.FileName);
            using (var sw = new StreamWriter(filePath))
                sw.Write(cfg.RawOutput);
        }

        private static void UploadResult(Config cfg, string baseDir)
        {
            var filePath = Path.Combine(baseDir, cfg.FileName);
            using (var sr = new StreamReader(filePath))
                cfg.RawOutput = sr.ReadToEnd();
            File.Delete(filePath);
        }

        private static IPEndPoint ParseIPEndPoint(string endPoint, int defaultPort)
        {
            var ep = endPoint.Split(':');
            if (ep.Length != 1 &&
                ep.Length != 2)
                throw new FormatException("Invalid endpoint format");

            IPAddress ip;
            if (!IPAddress.TryParse(ep[0], out ip))
                throw new FormatException("Invalid ip-adress");

            int port;
            if (ep.Length == 1)
                port = defaultPort;
            else if (!int.TryParse(ep[1], NumberStyles.None, NumberFormatInfo.CurrentInfo, out port))
                throw new FormatException("Invalid port");

            return new IPEndPoint(ip, port);
        }

        private static void Run(IInfQueue<Config> cached, Core core, string baseDir)
        {
#if DEBUG
            var threads = Environment.ProcessorCount;
#else
            var threads = Environment.ProcessorCount;
#endif

            var sch = new Scheduler<Config>(threads, cached);
            sch.OnLock += cfg => Console.WriteLine($"Started {cfg}");
            sch.OnPop += cfg => Console.WriteLine($"\t\t\tDone {cfg}");
            sch.OnWork += cfg => core.Process(cfg, baseDir);

            PrepareDir(baseDir);

            sch.Start();
            sch.WaitAll();
        }

        private static void PrepareDir(string baseDir)
        {
            if (!string.IsNullOrEmpty(baseDir))
                Directory.CreateDirectory(baseDir);
        }

        private static IInfQueue<Config> ConstructRoot()
        {
            var cmp = new BoundedComparer(2);

            var probs =
                new[]
                    {
                        new Rational(1, 2),
                        new Rational(1, 4),
                        new Rational(1, 8),
                        new Rational(1, 16),
                        new Rational(1, 32),
                        new Rational(1, 64)
                    };

            var root =
                new MergedInfQueue<Config>(cmp)
                    {
                        new MineBundle(cmp, 4, 2, "op"),
                        new MineBundle(cmp, 4, 2, "sl"),
                        new MineBundle(cmp, 4, 2, "fl"),
                        new ProbBundle(cmp, probs, "op", true),
                        new ProbBundle(cmp, probs, "sl", true),
                        new ProbBundle(cmp, probs, "fl", true)
                    };
            return root;
        }

        private static IInfQueue<Config> ConstructCache(IInfQueue<Config> root, string baseDir)
        {
            var cached = new CachedInfQueue<Config>(root) { BaseDir = baseDir };
            cached.OnSkipping += LoadCache;
            cached.OnPop += SaveCache;
            return SynInfQueue<Config>.Syncronize(cached);
        }

        private static void SaveCache(string filePath, Config cfg)
        {
            using (var sw = new StreamWriter(filePath, true))
            {
                sw.WriteLine();
                sw.WriteLine(cfg.Elapsed);
            }
        }

        private static void LoadCache(string filePath, Config cfg)
        {
            Console.WriteLine($"Skipping {cfg}");
            using (var sr = new StringReader(filePath))
            {
                var sp = sr.ReadToEnd()
                    ?.Split(
                            new[] { Environment.NewLine },
                            StringSplitOptions.RemoveEmptyEntries);
                if (sp?.Length != 2)
                    return;

                cfg.Elapsed = Convert.ToInt64(sp[1]);
            }
        }
    }
}
