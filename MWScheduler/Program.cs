using System;
using System.Globalization;
using System.IO;
using NDesk.Options;

namespace MWScheduler
{
    internal static class Program
    {
        private static void Main(string[] args)
        {
            const string theBaseDir = @"db/";
            const int defaultPort = 24680;

            int? port = null;
            var bare = false;
            string ip = null;
            var help = false;
            var useRational = false;

            var opt =
                new OptionSet
                    {
                        { "r|rational", "use rational core", v => useRational = v != null },
                        { "s|server=", "start server listening at {PORT}", (int v) => port = v },
                        { "b|bare", "don't run core; just listening", v => bare = v != null },
                        { "c|client=", "connect to server {ENDPOINT}", v => ip = ParseIPEndPoint(v, defaultPort) },
                        { "h|?|help", "show this message and exit", v => help = v != null }
                    };

            try
            {
                var extra = opt.Parse(args);

                if (help)
                {
                    ShowHelp(opt);
                    return;
                }

                if (extra.Count != 0)
                    throw new ApplicationException("extra command line argument(s):" + string.Join(" ", extra));

                if (!port.HasValue && bare)
                    throw new ApplicationException("cann't specify --bare without --server");

                if (port.HasValue &&
                    ip != null)
                    throw new ApplicationException("cann't --server and --client at the same time");

                if (useRational && bare)
                    throw new ApplicationException("cann't --bare and --rational at the same time");
            }
            catch (Exception e)
            {
                Console.Write("MWScheduler: ");
                Console.WriteLine(e.Message);
                Console.WriteLine("Try `MWScheduler --help' for more information.");
                return;
            }

            try
            {
                IInfQueue<Config> root;
                string baseDir;
                if (ip == null)
                {
                    baseDir = theBaseDir;
                    PrepareDir(baseDir);

                    root = ConstructRoot();
                    root = ConstructCache(root, baseDir);

                    if (port != null)
                    {
                        var server = ConstructServer(root, port.Value, baseDir);
                        server.Start();
                    }
                }
                else
                {
                    baseDir = "";

                    root = ConstructRemote(ip, baseDir);
                    root = SynInfQueue<Config>.Syncronize(root);
                }

                if (!bare)
                {
                    Core core;

                    if (useRational)
                        core = new RationalCore();
                    else
                        core = new DoubleCore();

                    Run(root, core, baseDir);
                }
                else
                    Console.Read();
            }
            catch (Exception e)
            {
                Console.Write("MWScheduler: ");
                Console.WriteLine(e.Message);
            }
        }

        private static RestfulInfQueueServer<Config> ConstructServer(IInfQueue<Config> cached, int defaultPort,
                                                                     string baseDir)
        {
            var server = new RestfulInfQueueServer<Config>(cached, defaultPort);
            server.OnPop += cfg => DownloadResult(cfg, baseDir);
            return server;
        }

        private static RestfulInfQueueClient<Config> ConstructRemote(string ip, string baseDir)
        {
            var remote = new RestfulInfQueueClient<Config>(ip);
            remote.OnPop += cfg => UploadResult(cfg, baseDir);
            return remote;
        }

        private static void ShowHelp(OptionSet opt)
        {
            Console.WriteLine("Usage: MWScheduler [OPTIONS]+");
            Console.WriteLine("Run MW1D program");
            Console.WriteLine();
            Console.WriteLine("Options:");
            opt.WriteOptionDescriptions(Console.Out);
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

        private static string ParseIPEndPoint(string endPoint, int defaultPort)
        {
            var ep = endPoint.Split(':');
            if (ep.Length != 1 &&
                ep.Length != 2)
                throw new FormatException("Invalid endpoint format");

            var host = ep[0];

            int port;
            if (ep.Length == 1)
                port = defaultPort;
            else if (!int.TryParse(ep[1], NumberStyles.None, NumberFormatInfo.CurrentInfo, out port))
                throw new FormatException("Invalid port");

            return $"{host}:{port}";
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
