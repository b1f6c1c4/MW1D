using System.Runtime.InteropServices;

namespace MWScheduler
{
    public class DoubleCore : Core
    {
        [DllImport("MW1D-double.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceT(int n, int m,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  [MarshalAs(UnmanagedType.LPStr)] string filePath);

        [DllImport("MW1D-double.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceP(int n, int pN, int pD,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  [MarshalAs(UnmanagedType.LPStr)] string filePath);

        protected override void CalculateT(int n, int m, string solver, string filePath) =>
            CoreInterfaceT(n, m, solver, filePath);

        protected override void CalculateP(int n, int pN, int pD, string solver, string filePath) =>
            CoreInterfaceP(n, pN, pD, solver, filePath);
    }
}
