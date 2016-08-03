using System;
using System.IO;

namespace MWScheduler
{
    internal static class Program
    {
        private static void Main(string[] args)
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

            const string baseDir = @"db/";

            var cached = new CachedInfQueue<Config>(root) { BaseDir = baseDir };
            cached.OnSkipping += LoadCache;
            cached.OnPop += SaveCache;

#if DEBUG
            var threads = 1;
#else
            var threads = Environment.ProcessorCount;
#endif

            var sch =
                new Scheduler<Config>(threads)
                    {
                        TheQueue = cached
                    };

            sch.OnLock += cfg => Console.WriteLine($"Started {cfg}");
            sch.OnPop += cfg => Console.WriteLine($"\t\t\tDone {cfg}");
            sch.OnWork += cfg => cfg.Process(baseDir);

            Directory.CreateDirectory(baseDir);

            sch.Start();
            sch.WaitAll();
        }

        private static void SaveCache(string filePath, Config cfg)
        {
            using (var sw = new StreamWriter(filePath, true))
            {
                sw.WriteLine();
                sw.WriteLine(cfg.Elapsed.Ticks);
            }
        }

        private static void LoadCache(string filePath, Config cfg)
        {
            using (var sr = new StringReader(filePath))
            {
                var sp = sr.ReadToEnd()
                    ?.Split(
                            new[] { Environment.NewLine },
                            StringSplitOptions.RemoveEmptyEntries);
                if (sp?.Length != 2)
                    return;

                cfg.Elapsed = new TimeSpan(Convert.ToInt64(sp[1]));
            }
        }
    }
}
