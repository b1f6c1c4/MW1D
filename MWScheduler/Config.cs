using System;
using System.Diagnostics;

namespace MWScheduler
{
    public class Config
    {
        public int Width { get; set; }

        public bool UseTotalMines { get; set; }

        public int TotalMines { get; set; }

        public int ProbNumerator { get; set; }

        public int ProbDenominator { get; set; }

        public string Strategy { get; set; }

        public TimeSpan Elapsed { get; set; }

        private string Argument
            =>
                UseTotalMines
                    ? $"{Width} {TotalMines} {Strategy}"
                    : $"{Width} {ProbNumerator}/{ProbDenominator} {Strategy}";

        public string Invoke(string fn)
        {
            var proc =
                new Process
                    {
                        StartInfo =
                            new ProcessStartInfo
                                {
                                    FileName = fn,
                                    Arguments = Argument,
                                    UseShellExecute = false,
                                    RedirectStandardOutput = true,
                                    CreateNoWindow = true
                                }
                    };
            proc.Start();
            var res = proc.StandardOutput.ReadToEnd();
            Elapsed = proc.ExitTime.Subtract(proc.StartTime);
            return res;
        }
    }
}
