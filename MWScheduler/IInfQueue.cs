namespace MWScheduler
{
    public interface IInfQueue<T>
        where T : class
    {
        T Top { get; }

        bool IsLocked { get; }

        T Lock();

        bool Pop(T obj);
    }
}
