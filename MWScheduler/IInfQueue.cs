namespace MWScheduler
{
    public interface IInfQueue<out T>
    {
        T Top { get; }

        void Pop();
    }
}
