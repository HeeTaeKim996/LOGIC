package Behaviour;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Publish_Subscriber
{
    /*
    ■ Publish-Subscriber 패턴
    Observer 패턴과 거의 같으나, 차이점은 발행자와 구독자 사이에 Broker 라는 클래스를 추가함으로써,
    발행자와 구독자의 관계가 더 유연해진다.

    발행자는 구독자를 직접 호출하지 않고, 구독자에 대한 정보도 갖고 있지 않다.
    단지 브로커에게 특정 방송(항목) 의 메세지를 전달하고,
    브로커가 자신이 갖고 있는 해당 방송을 구독 정보를 통해 구독자들에게 전달한다.

    추가로 이 패턴은 비동기 메세지 송신/수신 에서 옵저버 패턴에 비해 더 유리하기에, 비동기 송/수신 때 자주 사용된다
    (스레드풀을 Broker 가 유일하게 갖고 관리하기 때문)
     */

    public static void main(String[] args)
    {
        /*-----------------------
         *         EX 1
         * --------------------- */
        Broker broker = new Broker();

        NewsPublisher publisher = new NewsPublisher(broker);

        NewsSubscriber subscriber1 = new NewsSubscriber("Subscriber1");
        NewsSubscriber subscriber2 = new NewsSubscriber("Subscriber2");

        broker.subscribe("sports", subscriber1);
        broker.subscribe("weather", subscriber2);
        broker.subscribe("sports", subscriber2);

        publisher.publish(new Message("Liverpool won the match", "sports"));
        publisher.publish(new Message("It's sunny today", "weather"));

        /*-----------------------
         *         EX 2
         * --------------------- */
        EmailDeliveryService emailService = new EmailDeliveryService();

        MarketingDepartment marketing = new MarketingDepartment(emailService, "ProductLaunch");

        Customer customer1 = new IndividualCustomer("Customer1");
        Customer customer2 = new IndividualCustomer("Customer2");

        emailService.subscribe("ProductLaunch", customer1);
        emailService.subscribe("ProductLaunch", customer2);

        marketing.launchCampaign("New Product");

        Customer customer3 = new IndividualCustomer("Customer3");
        emailService.subscribe("ProductLaunch", customer3);

        marketing.launchCampaign("update");
        try
        {
            Thread.sleep(10000);
        }
        catch (InterruptedException e)
        {
            Thread.currentThread().interrupt();
        }

        emailService.shutdown();
    }

    /*-----------------------
     *         EX 1
     * --------------------- */
    static class Message
    {
        private String content;
        private String topic;

        public Message(String inContent, String inTopic)
        {
            content = inContent;
            topic = inTopic;
        }

        public String getContent(){ return content;}
        public String getTopic(){ return topic;}
    }

    static interface Publisher
    {
        void publish(Message message);
    }

    static interface Subscriber
    {
        void update(Message message);
    }

    static class Broker
    {
        private Map<String, List<Subscriber>> subscribers = new HashMap<>();

        public void subscribe(String topic, Subscriber subscriber)
        {
            subscribers.computeIfAbsent(topic,  k-> new ArrayList<>())
                            .add(subscriber);
        }

        public void publish(Message message)
        {
            List<Subscriber> topicSubscribers = subscribers.get(message.getTopic());
            for(Subscriber subscriber : topicSubscribers)
            {
                subscriber.update(message);
            }
        }
    }

    static class NewsPublisher implements Publisher
    {
        private Broker broker;
        public NewsPublisher(Broker inBroker)
        {
            broker = inBroker;
        }

        @Override
        public void publish(Message message)
        {
            System.out.printf("Publishing [%s] on topic : [%s]\n", message.getContent(),
                    message.getTopic());
            broker.publish(message);
        }
    }

    static class NewsSubscriber implements Subscriber
    {
        private String name;

        public NewsSubscriber(String inName)
        {
            name = inName;
        }

        @Override
        public void update(Message message)
        {
            System.out.printf("[%s] received [%s] on topic [%s]\n", name, message.getContent(),
                    message.getTopic());
        }
    }






    /*-----------------------
     *         EX 2
     * --------------------- */


    static interface Customer
    {
        void receiveEmail(String message);
    }

    static class IndividualCustomer implements Customer
    {
        private String name;

        public IndividualCustomer(String inName)
        {
            name = inName;
        }

        @Override
        public void receiveEmail(String message)
        {
            System.out.printf("[%s] is receiving email async : [%s]\n", name, message);
            try
            {
                Thread.sleep(5000);
            }
            catch(InterruptedException e)
            {
                Thread.currentThread().interrupt();
            }

            System.out.printf("[%s] finished reading email : [%s]\n", name, message);
        }
    }

    static class EmailDeliveryService
    {
        private Map<String, List<Customer>> customerGroups = new HashMap<>();
        private ExecutorService executor = Executors.newCachedThreadPool();
        // ExecutorService : 매번 new Thread 하는 비용을 줄이기 위해, 스레드풀을 관리하는 객체

        public void subscribe(String eventType, Customer customer)
        {
            customerGroups.computeIfAbsent(eventType,
                    k->new ArrayList<Customer>()).add(customer);
        }

        public void sendEmails(String eventType, String message)
        {
            List<Customer> customers = customerGroups.get(eventType);
            if(customers != null)
            {
                for(Customer customer : customers)
                {
                    executor.submit(()-> customer.receiveEmail(message));
                    // 백그라운드에서 비동기로 실행
                }
            }
        }
        public void shutdown() { executor.shutdown();}
    }

    static class MarketingDepartment
    {
        private EmailDeliveryService emailDeliveryService;
        private String eventType;

        public MarketingDepartment(EmailDeliveryService inEmailDeliverySErvice, String inEventType)
        {
            emailDeliveryService = inEmailDeliverySErvice;
            eventType = inEventType;
        }

        public void launchCampaign(String message)
        {
            System.out.printf("Launching campaign: [%s]\n", message);
            emailDeliveryService.sendEmails(eventType, message);
        }
    }
}
