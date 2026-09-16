package Behaviour;

import java.util.Stack;

public class Command
{
    /*----------------------------------------------------
    ■ Command 패턴
    명령들을 객체로 만들어 두고, 컬렉션에 저장하여,
    원할 때 실행/실행취소/다시실행 을 처리하는 패턴

    문서 편집기의 뒤로가기/앞으로가기 가 대표적인 예시

    interface 에 execute, undo 함수를 구현하고,
    명령어 객체들이 interface 를 상속하여 내용을 구현하고,
    이 객체들을 undoStack, redoStack 에 저장하여 사용
     ----------------------------------------------------*/
    public static void main(String[] args)
    {
        /*-----------------------
         *         EX 1
         * --------------------- */
        Light roomLight = new Light();

        Command1 lightOn = new LightOnCommand(roomLight);

        Command1 lightOff = new LightOffCommand(roomLight);

        RemoteControl remote = new RemoteControl();

        remote.setCommand(lightOn);
        remote.pressButton();

        remote.setCommand(lightOff);
        remote.pressButton();



        /*-----------------------
         *         EX 2
         * --------------------- */
        TextEditor editor = new TextEditor();
        TextEditorInvoker invoker = new TextEditorInvoker();

        Command2 cm1 = new InsertTextCommand(editor, "Hello, ", 0);
        invoker.executeCommand(cm1);

        Command2 cm2 = new InsertTextCommand(editor, "Harry!", 7);
        invoker.executeCommand(cm2);

        System.out.printf("Curr text : %s\n",editor.getContent());

        invoker.undo();
        System.out.printf("After undo : %s\n", editor.getContent());

        invoker.redo();
        System.out.printf("After redo : %s\n", editor.getContent());

        Command2 cm3 = new DeleteTextCommand(editor, 0, 7);
        invoker.executeCommand(cm3);

        System.out.printf("After delete : %s\n", editor.getContent());

        invoker.undo();
        System.out.printf("After undo : %s\n", editor.getContent());
    }

    /*-----------------------
     *         EX 1
     * --------------------- */
    public static class Light
    {
        public void turnOn()
        {
            System.out.println("불 켜기");
        }

        public void turnOff()
        {
            System.out.println("불 끄기");
        }
    }

    public static interface Command1
    {
        void execute();
    }


    public static class LightOnCommand implements Command1
    {
        private Light light;

        public LightOnCommand(Light InLight)
        {
            light = InLight;
        }

        @Override
        public void execute()
        {
            light.turnOn();
        }
    }

    public static class LightOffCommand implements Command1
    {
        private Light light;

        public LightOffCommand(Light InLight)
        {
            light = InLight;
        }

        @Override
        public void execute()
        {
            light.turnOff();
        }
    }

    public static class RemoteControl
    {
        private Command1 command;

        public void setCommand(Command1 InCommand)
        {
            command = InCommand;
        }

        public void pressButton()
        {
            command.execute();
        }
    }



    /*-----------------------
     *         EX 2
     * --------------------- */

    public static class TextEditor
    {
        private StringBuilder content;

        public TextEditor()
        {
            content = new StringBuilder();
        }

        public void insertText(String text, int position)
        {
            content.insert(position, text);
        }

        public void deleteText(int position, int length)
        {
            content.delete(position, position + length);
        }

        public String getTextSubstring(int start, int end)
        {
            return content.substring(start, end);
        }

        public String getContent()
        {
            return content.toString();
        }
    }

    public static interface Command2
    {
        void execute();
        void undo();
    }

    public static class InsertTextCommand implements Command2
    {
        private TextEditor editor;
        private String text;
        private int position;

        public InsertTextCommand(TextEditor InEditor, String InText, int InPosition)
        {
            editor = InEditor;
            text = InText;
            position = InPosition;
        }

        @Override
        public void execute()
        {
            editor.insertText(text, position);
        }

        @Override
        public void undo()
        {
            editor.deleteText(position, text.length());
        }
    }

    public static class DeleteTextCommand implements Command2
    {
        private TextEditor editor;
        private String deletedText;
        private int position;

        public DeleteTextCommand(TextEditor InEditor, int InPosition, int InLength)
        {
            editor = InEditor;
            position = InPosition;
            deletedText = editor.getTextSubstring(InPosition, InPosition + InLength);
        }

        @Override
        public void execute()
        {
            editor.deleteText(position, deletedText.length());
        }

        @Override
        public void undo()
        {
            editor.insertText(deletedText, position);
        }
    }

    public static class TextEditorInvoker
    {
        private Stack<Command2> undoStack = new Stack<>();
        private Stack<Command2> redoStack = new Stack<>();

        public void executeCommand(Command2 command)
        {
            command.execute();
            undoStack.push(command);
            redoStack.clear();
        }

        public void undo()
        {
            if(undoStack.isEmpty() == false)
            {
                Command2 command = undoStack.pop();
                command.undo();
                redoStack.push(command);
            }
        }

        public void redo()
        {
            if(redoStack.isEmpty() == false)
            {
                Command2 command = redoStack.pop();
                command.execute();
                undoStack.push(command);
            }
        }

    }

}
