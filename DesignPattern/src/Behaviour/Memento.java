package Behaviour;

import javax.print.Doc;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

public class Memento
{
    /*----------------------------------------------------
    ■ Memento 패턴
    객체의 내부 상태를 스냅샷 으로 저장했다가,
    필요할 때 복원하는 패턴

    게임의 세이브 시스템, 게임의 리플레이 시스템 에서 주로 사용

    커맨드 패턴과 용도와 목적, 방법이 유사하다.
    커맨드 패턴은 저장하는 단위가 액션(execute, undo) 인 반면,
    메멘토 패턴은 저장하는 단위가 저장 객체 자체 이다.
     ----------------------------------------------------*/
    public static void main(String[] args)
    {
        /*-----------------------
         *         EX 1
         * --------------------- */
        Game game = new Game();
        GameCareTaker caretaker = new GameCareTaker();

        game.set("Level 1", 100);
        caretaker.add(game.save());

        game.set("Level 2", 200);
        caretaker.add(game.save());

        game.set("Level 3", 300);

        game.restore(caretaker.get(1));
        game.restore(caretaker.get(0));


        /*-----------------------
         *         EX 2
         * --------------------- */
        Editor editor = new Editor();

        editor.write("Hello, ");
        editor.write("Bori");
        System.out.printf("Curr : %s\n", editor.getContent());

        editor.undo();
        System.out.printf("undo : %s\n", editor.getContent());

        editor.redo();
        System.out.printf("redo : %s\n", editor.getContent());



    }

    /*-----------------------
     *         EX 1
     * --------------------- */
    static class GameMemento
    {
        private String level;
        private int score;

        public GameMemento(String InLevel, int InScore)
        {
            level = InLevel;
            score = InScore;
        }

        public String getLevel() { return level;}
        public int getScore() { return score;}
    }

    static class Game
    {
        private String level;
        private int score;

        public void set(String InLevel, int InScore)
        {
            level = InLevel;
            score = InScore;
            System.out.printf("Game State set to Level[%s], Score[%d]\n", level, score);
        }

        public GameMemento save()
        {
            return new GameMemento(level, score);
        }

        public void restore(GameMemento memento)
        {
            level = memento.getLevel();
            score = memento.getScore();
            System.out.printf("Game state restored to Level[%s], Score[%d]\n", level, score);
        }
    }

    static class GameCareTaker
    {
        private List<GameMemento> mementoList = new ArrayList<>();

        public void add(GameMemento state)
        {
            mementoList.add(state);
        }

        public GameMemento get(int index)
        {
            return mementoList.get(index);
        }
    }




    /*-----------------------
     *         EX 2
     * --------------------- */
    static class DocumentMemento
    {
        private final String content;

        public DocumentMemento(String InContent)
        {
            content = InContent;
        }

        public String getContent() { return content;}
    }

    static class Document
    {
        private StringBuilder content;

        public Document()
        {
            content = new StringBuilder();
        }

        public void write(String text)
        {
            content.append(text);
        }

        public String getContent()
        {
            return content.toString();
        }

        public DocumentMemento save()
        {
            return new DocumentMemento(content.toString());
        }

        public void restore(DocumentMemento memento)
        {
            content = new StringBuilder(memento.getContent());
        }
    }

    static class DocumentHistory
    {
        private final Stack<DocumentMemento> history = new Stack<>();

        public void push(DocumentMemento memento)
        {
            history.push(memento);
        }

        public DocumentMemento pop()
        {
            if(history.isEmpty() == false)
            {
                return history.pop();
            }

            return null;
        }

        public void clear()
        {
            history.clear();
        }
    }


    // Client Code
    static class Editor
    {
        private final Document document;
        private final DocumentHistory undos;
        private final DocumentHistory redos;

        public Editor()
        {
            document = new Document();
            undos = new DocumentHistory();
            redos = new DocumentHistory();
        }

        public void write(String text)
        {
            undos.push(document.save());
            document.write(text);
            redos.clear();
        }

        public void undo()
        {
            redos.push(document.save());
            DocumentMemento memento = undos.pop();
            if(memento != null)
            {
                document.restore(memento);
            }
        }
        public void redo()
        {
            DocumentMemento memento = redos.pop();
            if(memento != null)
            {
                document.restore(memento);
            }
        }

        public String getContent()
        {
            return document.getContent();
        }
    }
}
