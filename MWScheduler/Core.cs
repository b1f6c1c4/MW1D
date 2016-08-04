using System.Diagnostics;
using System.IO;

namespace MWScheduler
{
    public abstract class Core
    {
        protected abstract void CalculateT(int n, int m, string solver, string filePath);

        protected abstract void CalculateP(int n, int pN, int pD, string solver, string filePath);

        public void Process(Config cfg, string baseDir)
        {
            var filePath = Path.Combine(baseDir, cfg.FileName);
            var stopwatch = Stopwatch.StartNew();
            if (cfg.UseTotalMines)
                CalculateT(
                           cfg.Width,
                           cfg.TotalMines,
                           cfg.Strategy,
                           filePath);
            else
                CalculateP(
                           cfg.Width,
                           cfg.Probability.Numerator,
                           cfg.Probability.Denominator,
                           cfg.Strategy,
                           filePath);
            stopwatch.Stop();
            cfg.Elapsed = stopwatch.ElapsedTicks;
        }
    }
}
