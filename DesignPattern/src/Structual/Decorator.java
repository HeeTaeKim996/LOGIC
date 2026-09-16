package Structual;

public class Decorator
{
    /*-------------------------------------------------------------------
    ■ Decorator 패턴
    중복 조합으로 사용가능한 '첨가물' 들을 다루는 패턴

    카페 메뉴에 설탕추가, 우유추가 가 메뉴로 추가된다 하자.
    그럼 (), (설탕추가), (우유추가), (설탕추가/우유추가) 의 조합으로 인해,
    기존 매뉴가 n 개 였다면, 메뉴 수는 n * 4 가 된다.
    Decorator 패턴은 이를 첨가물 로 처리하여, 구조를 간단하게 한다.

    패턴의 구조는 고정적이다.
    1) 구현체와 데코레이터가 공통으로 구현할 interface를 선언한다.
    2) 구현체는 interface 를 일반 클래스로 구현한다. ( 위 예시의 일반 메뉴들.. 아메리카노, 라떼..)
    3) 데코레이터는 interface 를 abstract class 로 구현하며, 필드에 interface 를 두고,
       생성자 때 interface를 받아 필드에 할당한다.
    4) 첨가물들(위 예시의 설탕, 우유추가..)은 데코레이터를 일반 클래스로 구현하며,
       루트 interface 의 함수들 또한 @Override 로 구현한다.

    ==> interface, abstract class 는 Concrete 가 될 수 없기에,
        위 데코레이터 생성자 때 받을 수 있는 interface 는 (구현체 || 첨가물) 만 올 수 있다.
     -------------------------------------------------------------------*/

    public static void main(String[] args)
    {
        /*-----------------------
         *         EX 1
         * --------------------- */
        Coffee simpleCoffee = new SimpleCoffee();
        System.out.printf("%s : $%s\n", simpleCoffee.getDescription(), simpleCoffee.getCost());

        Coffee milkCoffee = new MlikDecorator(new SimpleCoffee());
        System.out.printf("%s : %s\n", milkCoffee.getDescription(), milkCoffee.getCost());

        Coffee mlikAndSugarCoffee = new SugarDecorator(new MlikDecorator(new SimpleCoffee()));
        System.out.printf("%s : %s\n", mlikAndSugarCoffee.getDescription(), mlikAndSugarCoffee.getCost());



        /*-----------------------
         *         EX 2
         * --------------------- */
        Text text = new PlainText("Hello Harry");
        System.out.println("PlainText : " + text.getContent());

        text = new BoldDecorator(text);
        System.out.printf("Bold Text : %s\n", text.getContent());

        text = new ItalicDecorator(text);
        System.out.printf("Bold And Italic : %s\n", text.getContent());

        Text anotherText = new BoldDecorator(new ItalicDecorator(new PlainText("Hi Bori")));
        System.out.printf("Italic And Bold : %s\n", anotherText.getContent());
    }

    /*-----------------------
     *         EX 1
     * --------------------- */
    static interface Coffee
    {
        String getDescription();
        double getCost();
    }

    static class SimpleCoffee implements Coffee
    {
        @Override
        public String getDescription()
        {
            return "Simple Coffee";
        }

        @Override
        public double getCost() {
            return 5.0;
        }
    }

    static abstract class CoffeeDecorator implements Coffee
    {
        protected Coffee decoratedCoffee;

        public CoffeeDecorator(Coffee coffee)
        {
            decoratedCoffee = coffee;
        }

        @Override
        public String getDescription()
        {
            return decoratedCoffee.getDescription();
        }

        @Override
        public double getCost() {
            return decoratedCoffee.getCost();
        }
    }

    static class MlikDecorator extends CoffeeDecorator
    {
        public MlikDecorator(Coffee coffee)
        {
            super(coffee);
        }

        public String getDescription()
        {
            return super.getDescription() + ", milk";
        }
        public double getCost()
        {
            return super.getCost() + 1.5;
        }
    }

    static class SugarDecorator extends CoffeeDecorator
    {
        public SugarDecorator(Coffee coffee)
        {
            super(coffee);
        }

        public String getDescription()
        {
            return super.getDescription() + ", Sugar";
        }
        public double getCost()
        {
            return super.getCost() + 0.5;
        }
    }






    /*-----------------------
     *         EX 2
     * --------------------- */
    static interface Text
    {
        String getContent();
    }

    static class PlainText implements Text
    {
        private String content;

        public PlainText(String InContent)
        {
            content = InContent;
        }

        @Override
        public String getContent()
        {
            return content;
        }
    }

    static abstract class TextDecorator implements Text
    {
        protected Text decoratedText;

        public TextDecorator(Text text)
        {
            decoratedText = text;
        }

        @Override
        public String getContent()
        {
            return decoratedText.getContent();
        }
    }

    static class BoldDecorator extends TextDecorator
    {
        public BoldDecorator(Text text)
        {
            super(text);
        }

        @Override
        public String getContent()
        {
            return "<b>" + decoratedText.getContent() + "</b>";
        }
    }

    static class ItalicDecorator extends TextDecorator
    {
        public ItalicDecorator(Text text)
        {
            super(text);
        }

        @Override
        public String getContent()
        {
            return "<i>" + decoratedText.getContent() + "</i>";
        }
    }

}
