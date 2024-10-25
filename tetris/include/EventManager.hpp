#include <any>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class EventManager {
public:
    using EventCallback = std::function<void(const std::any &)>;
    using SubscriptionId = size_t;

    SubscriptionId subscribe(const std::string &eventType,
                             EventCallback callback) {
        SubscriptionId subscriptionId{m_nextSubscriptionId++};
        m_subscribers[eventType][subscriptionId] = std::move(callback);

        return subscriptionId;
    }

    void unsubscribe(const std::string &eventType,
                     SubscriptionId subscriptionId) {
        const auto subscriberIter{m_subscribers.find(eventType)};

        if (subscriberIter == m_subscribers.end()) {
            return;
        }

        subscriberIter->second.erase(subscriptionId);

        if (subscriberIter->second.empty()) {
            m_subscribers.erase(eventType);
        }
    }

    void publish(const std::string &eventType, const std::any &payload) {
        const auto subscriberIter{m_subscribers.find(eventType)};

        if (subscriberIter == m_subscribers.end()) {
            return;
        }

        for (const auto &[subscriptionId, callback] : subscriberIter->second) {
            callback(payload);
        }
    }

private:
    using SubscriberMap = std::unordered_map<SubscriptionId, EventCallback>;
    std::unordered_map<std::string, SubscriberMap> m_subscribers;
    SubscriptionId m_nextSubscriptionId{0};
};
