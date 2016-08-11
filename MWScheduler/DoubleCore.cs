using System.Runtime.InteropServices;

namespace MWScheduler
{
    public class DoubleCore : Core
    {
        [DllImport("MW1D-double.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceT(int n, int m,
                                                  [MarshalAs(UnmanagedType.U1)] bool nR,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  [MarshalAs(UnmanagedType.LPStr)] string filePath);

        [DllImport("MW1D-double.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void CoreInterfaceP(int n, int pN, int pD,
                                                  [MarshalAs(UnmanagedType.U1)] bool nR,
                                                  [MarshalAs(UnmanagedType.LPStr)] string solver,
                                                  [MarshalAs(UnmanagedType.LPStr)] string filePath);

        protected override void CalculateT(int n, int m, bool nR, string solver, string filePath) =>
            CoreInterfaceT(n, m, nR, solver, filePath);

        protected override void CalculateP(int n, int pN, int pD, bool nR, string solver, string filePath) =>
            CoreInterfaceP(n, pN, pD, nR, solver, filePath);
    }
}
