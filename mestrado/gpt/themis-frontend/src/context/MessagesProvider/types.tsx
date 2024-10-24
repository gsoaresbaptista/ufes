export interface Message {
  id: string;
  user_id: string;
  question: string;
  answer: string;
  created_at: string;
  unix: number;
}

export interface MessageContextProps {
  messages: Message[];
  loading: boolean;
  setMessages: (messages: Message[]) => void;
  question: (text: string) => void;
  continueAnswer: () => void;
  updateMessages: () => Promise<void>;
  clearMessages: () => Promise<void>;
}

export interface IMessagesProvider {
  children: JSX.Element;
}
