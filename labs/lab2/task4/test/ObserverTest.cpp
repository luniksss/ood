#include <gtest/gtest.h>
#include <vector>
#include "../SWeatherInfo.h"
#include "../IObserver.h"
#include "../Observable/CWeatherData.h"

class TestObserver : public IObserver<SWeatherInfo>
{
public:
    std::vector<const void*> notifications;
    void Update(SWeatherInfo const& data, const IObservable<SWeatherInfo>* sender) override
    {
        notifications.push_back(sender);
    }
};

TEST(WeatherStationDuoTest, ObserverReceivesNotificationFromTwoSources)
{
    CWeatherData weatherIn;
    CWeatherData weatherOut;

    TestObserver observer;

    weatherIn.RegisterObserver(observer, 1);
    weatherOut.RegisterObserver(observer, 1);

    weatherIn.SetMeasurements(20.0, 40.0, 750.0);
    weatherOut.SetMeasurements(10.0, 80.0, 740.0);

    ASSERT_EQ(observer.notifications.size(), 2);
    EXPECT_TRUE(observer.notifications[0] == &weatherIn || observer.notifications[0] == &weatherOut);
    EXPECT_TRUE(observer.notifications[1] == &weatherIn || observer.notifications[1] == &weatherOut);
    EXPECT_NE(observer.notifications[0], observer.notifications[1]);
}
