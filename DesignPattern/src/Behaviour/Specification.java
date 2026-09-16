package Behaviour;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class Specification
{
    /*
    ■ Specification
    boolean 판별 함수 하나만 선언한 interface를 상속한 클래스들을 기반으로,
    AND OR NOT NOR 등으로 해당 Concrete 클래스들을 조합하여
    새로운 판별 클래스를 만들어내는 패턴

    EX1 보다는 EX2 를 통해 구조를 이해하자

    isSatisfiedBy 함수를 선언한 interface 를 만들고,
    1) 인터페이스를 구현한 개별 판별 클래스
    2) 1) 클래스들을 조합하는 목적으로 interface 를 구현하는 AND,OR,XOR,NOT 클래스
    들을 구현한다. 2) 는 구조가 고정이며, 1) 은 유저가 추가하여, 2) 를 통해 여러 조합을 만들 수 있다
     */

    public static void main(String[] args)
    {
        /*-----------------------
         *         EX 1
         * --------------------- */
        Specification1 evenSpec = new EvenSpecification();
        Specification1 rangeSpec = new RangeSpecification(10, 20);

        Specification1 eventAndInRangeSpec = evenSpec.and(rangeSpec);

        int number = 24;

        System.out.printf("Even : %b\n", evenSpec.isSatisfiedBy(number));
        System.out.printf("In Range 10 - 20 : %b\n", rangeSpec.isSatisfiedBy(number));
        System.out.printf("Even and In Range 10 - 20 : %b\n", eventAndInRangeSpec.isSatisfiedBy(number));





        /*-----------------------
         *         EX 2
         * --------------------- */
        List<Product> products = Arrays.asList(
                new Product("Laptop", "Electronics", 1200, 5),
                new Product("Smartphone", "Electronics", 800, 0),
                new Product("Headphones", "Electronics", 200, 10),
                new Product("Book", "Literature", 20, 50)
        );

        Specification2 electronicsSpec = new CategorySpec("Electronics");
        Specification2 inStockSpec = new InStockSpec();
        Specification2 expensiveSpec = new PriceSpec(500);

        Specification2 electronicInStock
                = new AndSpec(electronicsSpec, inStockSpec);
        Specification2 electronicOrInStock
                = new OrSpec(electronicsSpec, inStockSpec);
        Specification2 notExpensive = new NotSpec(expensiveSpec);

        System.out.println("Electronics in stock:");
        ProductFilter.printProducts(
                ProductFilter.filter(products, electronicInStock));

        System.out.println("\nElectronics or items in stock:");
        ProductFilter.printProducts(
                ProductFilter.filter(products, electronicOrInStock));

        System.out.println("\nNot expensive items:");
        ProductFilter.printProducts(
                ProductFilter.filter(products, notExpensive));
    }


    /*-----------------------
     *         EX 1
     * --------------------- */
    public static interface Specification1
    {
        boolean isSatisfiedBy(int number);

        default Specification1 and(Specification1 other)
        {
            return number-> this.isSatisfiedBy(number)
                    && other.isSatisfiedBy(number);
            // 위 두 함수가 true 일 때에 true를 반환하는 함수.
            // default + and 조합으로 저렇게 두개의 함수를 사용해서 비교하는 목적인듯
        }
    }

    static class EvenSpecification implements Specification1
    {
        @Override
        public boolean isSatisfiedBy(int number)
        {
            return number % 2 == 0;
        }
    }

    static class RangeSpecification implements Specification1
    {
        private int min;
        private int max;

        public RangeSpecification(int inMin, int inMax)
        {
            min = inMin;
            max = inMax;
        }

        @Override
        public boolean isSatisfiedBy(int number)
        {
            return number >= min && number <= max;
        }
    }







    /*-----------------------
     *         EX 2
     * --------------------- */
    public static class Product
    {
        private String name;
        private String category;
        private int price;
        private int stock;

        public Product(String inName, String inCategory, int inPrice, int inStock)
        {
            name = inName;
            category = inCategory;
            price = inPrice;
            stock = inStock;
        }

        public String getName(){ return name;}
        public String getCategory(){ return category;}
        public int getPrice(){ return price;}
        public int getStock(){ return stock;}
    }

    public static interface Specification2
    {
        boolean isSatisfiedBy(Product item);
    }


    public static class CategorySpec implements Specification2
    {
        private String category;

        public CategorySpec(String inCategory)
        {
            category = inCategory;
        }

        @Override
        public boolean isSatisfiedBy(Product item)
        {
            return item.getCategory().equals(category);
        }
    }

    public static class PriceSpec implements Specification2
    {
        private int price;

        public PriceSpec(int inPrice)
        {
            price = inPrice;
        }

        @Override
        public boolean isSatisfiedBy(Product item)
        {
            return item.getPrice() == price;
        }
    }

    public static class InStockSpec implements Specification2
    {
        @Override
        public boolean isSatisfiedBy(Product item)
        {
            return item.getStock() > 0;
        }
    }

    public static class AndSpec implements Specification2
    {
        private Specification2 spec1;
        private Specification2 spec2;

        public AndSpec(Specification2 in1, Specification2 in2)
        {
            spec1 = in1;
            spec2 = in2;
        }

        @Override
        public boolean isSatisfiedBy(Product item)
        {
            return spec1.isSatisfiedBy(item) && spec2.isSatisfiedBy(item);
        }
    }

    public static class OrSpec implements Specification2
    {
        private Specification2 spec1;
        private Specification2 spec2;

        public OrSpec(Specification2 in1, Specification2 in2)
        {
            spec1 = in1;
            spec2 = in2;
        }

        @Override
        public boolean isSatisfiedBy(Product item)
        {
            return spec1.isSatisfiedBy(item) || spec2.isSatisfiedBy(item);
        }
    }

    public static class NotSpec implements Specification2
    {
        private Specification2 spec1;

        public NotSpec(Specification2 in1)
        {
            spec1 = in1;
        }

        @Override
        public boolean isSatisfiedBy(Product item)
        {
            return spec1.isSatisfiedBy(item) == false;
        }
    }


    public static class ProductFilter
    {
        public static List<Product> filter(List<Product> items, Specification2 spec)
        {
            return items.stream().filter(spec::isSatisfiedBy)
                    .collect(Collectors.toList());
        }

        public static void printProducts(List<Product> products)
        {
            products.forEach(p->System.out.printf
                    ("%s - %s - $%d - stock : %d\n", p.getName(), p.getCategory(),
                            p.getPrice(), p.getStock()));
        }
    }
}
