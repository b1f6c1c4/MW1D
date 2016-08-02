using System.Diagnostics;
using System.Runtime.InteropServices;

namespace MWScheduler
{
    public static class DllInterface
    {
        [DllImport("MW1D.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceT(int n, int m,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  out long num, out long den);

        [DllImport("MW1D.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceP(int n, int pN, int pD,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  out long num, out long den);

        public static void Process(this Config cfg)
        {
            long num, den;
            var stopwatch = Stopwatch.StartNew();
            if (cfg.UseTotalMines)
                CoreInterfaceT(
                               cfg.Width,
                               cfg.TotalMines,
                               cfg.Strategy,
                               out num,
                               out den);
            else
                CoreInterfaceP(
                               cfg.Width,
                               cfg.Probability.Numerator,
                               cfg.Probability.Denominator,
                               cfg.Strategy,
                               out num,
                               out den);
            stopwatch.Stop();
            cfg.Elapsed = stopwatch.Elapsed;
            cfg.Result = new LongRational(num, den);
        }
    }
}
