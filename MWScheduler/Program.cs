using System;
using System.IO;

namespace MWScheduler
{
    internal static class Program
    {
        private static void Main(string[] args)
        {
            var exePath = AppDomain.CurrentDomain.BaseDirectory + @"MW1D.exe";

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

            var sch =
                new Scheduler<Config>(Environment.ProcessorCount)
                    {
                        TheQueue =
                            new MergedInfQueue<Config>(cmp)
                                {
                                    new MineBundle(cmp, 4, 2, "op"),
                                    new MineBundle(cmp, 4, 2, "sl"),
                                    new MineBundle(cmp, 4, 2, "fl"),
                                    new ProbBundle(cmp, probs, "op", true),
                                    new ProbBundle(cmp, probs, "sl", true),
                                    new ProbBundle(cmp, probs, "fl", true)
                                }
                    };

            sch.OnPop += cfg => Console.WriteLine($"Started {cfg}");
            sch.OnWork += cfg => Operate(cfg, exePath);

            sch.Start();
            sch.WaitAll();
        }

        private static void Operate(Config cfg, string exePath)
        {
            if (LoadCache(cfg))
                return;

            var res = cfg.Invoke(exePath);
            SaveCache(cfg, res);
        }

        private static void SaveCache(Config cfg, string res)
        {
            using (var sw = new StreamWriter(cfg.GetFileName()))
            {
                sw.WriteLine(res);
                sw.WriteLine(cfg.Elapsed.Ticks);
            }
        }

        private static bool LoadCache(Config cfg)
        {
            if (!File.Exists(cfg.GetFileName()))
                return false;

            using (var sr = new StringReader(cfg.GetFileName()))
            {
                var sp = sr.ReadToEnd()
                    ?.Split(
                            new[] { Environment.NewLine },
                            StringSplitOptions.RemoveEmptyEntries);
                if (sp?.Length != 2)
                    return false;

                cfg.Elapsed = new TimeSpan(Convert.ToInt64(sp[1]));
                return true;
            }
        }
    }
}
