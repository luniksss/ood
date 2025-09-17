#include <gtest/gtest.h>
#include "../Observable/CObservable.h"

template <typename T>
class TestObserver : public IObserver<T>
{
public:
    int updateCount = 0;

    void Update(const T& data) override
    {
        ++updateCount;
    }
};

template <typename T>
class SelfRemovingObserver : public IObserver<T>
{
public:
    explicit SelfRemovingObserver(CObservable<T>& observable)
        : m_observable(observable) {}

    void Update(const T& data) override
    {
        m_observable.RemoveObserver(*this);
        removed = true;
    }

    bool removed = false;

private:
    CObservable<T>& m_observable;
};

struct DummyData
{
    int value;
};

class ObservableTest : public testing::Test
{
protected:
    class MyObservable : public CObservable<DummyData>
    {
    public:
        void SetData(int v)
        {
            m_data.value = v;
            NotifyObservers();
        }

        [[nodiscard]] DummyData GetChangedData() const override
        {
            return m_data;
        }

    private:
        DummyData m_data{};
    };
};

TEST_F(ObservableTest, SelfRemovingObserverIsRemovedSafely)
{
    MyObservable observable;

    TestObserver<DummyData> observer1;
    SelfRemovingObserver<DummyData> selfRemovingObserver(observable);
    TestObserver<DummyData> observer2;

    observable.RegisterObserver(observer1);
    observable.RegisterObserver(selfRemovingObserver);
    observable.RegisterObserver(observer2);

    observable.SetData(42);

    // observer1 и observer2 получили обновление
    EXPECT_EQ(observer1.updateCount, 1);
    EXPECT_EQ(observer2.updateCount, 1);

    // selfRemovingObserver удалил себя во время Update
    EXPECT_TRUE(selfRemovingObserver.removed);

    // При повторном уведомлении selfRemovingObserver не должен обновляться
    observer1.updateCount = 0;
    observer2.updateCount = 0;
    observable.SetData(100);

    EXPECT_EQ(observer1.updateCount, 1);
    EXPECT_EQ(observer2.updateCount, 1);
    EXPECT_TRUE(selfRemovingObserver.removed);
}
