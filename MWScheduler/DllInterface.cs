using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;

namespace MWScheduler
{
    public static class DllInterface
    {
        [DllImport("MW1D.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceT(int n, int m,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  [MarshalAs(UnmanagedType.LPStr)] string filePath);

        [DllImport("MW1D.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceP(int n, int pN, int pD,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  [MarshalAs(UnmanagedType.LPStr)] string filePath);

        public static void Process(this Config cfg, string baseDir)
        {
            var filePath = Path.Combine(baseDir, cfg.FileName);
            var stopwatch = Stopwatch.StartNew();
            if (cfg.UseTotalMines)
                CoreInterfaceT(
                               cfg.Width,
                               cfg.TotalMines,
                               cfg.Strategy,
                               filePath);
            else
                CoreInterfaceP(
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
