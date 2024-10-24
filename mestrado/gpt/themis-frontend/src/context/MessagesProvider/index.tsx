import { createContext, useEffect, useState } from "react";
import { Message, IMessagesProvider, MessageContextProps } from "./types";
import {
  Clear,
  GetLastMessage,
  LoadMessages,
  Question,
} from "@/services/operations";
import { getMessagesLocalStorage, setMessagesLocalStorage } from "./utils";
import { useAuth } from "../AuthProvider/useAuth";
import { formatDate } from "./utils";

export const MessageContext = createContext<MessageContextProps>(
  {} as MessageContextProps
);

const delay = (ms: number) => new Promise((res) => setTimeout(res, ms));

const createFakeMessage = (question: string = "", answer: string = "") => {
  return {
    id: "",
    user_id: "",
    question: question,
    answer: answer,
    created_at: formatDate(new Date()),
  };
};

interface IDependencies {
  question: string;
  messages: Message[];
  continue: boolean;
}

export const MessageProvider = ({ children }: IMessagesProvider) => {
  const auth = useAuth();
  const [loading, setLoading] = useState<boolean>(false);
  const [messages, setMessages] = useState<Message[]>([]);
  const [questionText, setQuestionText] = useState<string>("");
  const [continueText, setContinueText] = useState<boolean>(false);
  const [dependencies, setDependencies] = useState<IDependencies>({
    question: "",
    messages: messages,
    continue: false,
  });

  const updateMessages = async () => {
    try {
      const messages = await LoadMessages();
      setMessages(messages);
      setMessagesLocalStorage(messages);
    } catch (error) {
      auth.logout();
    }
  };

  const clearMessages = async () => {
    try {
      await Clear();
      setMessages([]);
      setMessagesLocalStorage([]);
    } catch (error) {
      auth.logout();
    }
  };

  useEffect(() => {
    let messages = getMessagesLocalStorage();

    if (!messages) {
      updateMessages();
    } else {
      setMessages(messages);
    }
  }, []);

  useEffect(() => {
    setDependencies((prev: IDependencies) => {
      return (questionText !== prev.question && messages !== prev.messages) ||
        prev.continue !== continueText
        ? ({
            question: questionText,
            messages,
            continue: continueText,
          } as IDependencies)
        : prev;
    });
  }, [questionText, messages, continueText]);

  useEffect(() => {
    if (dependencies?.question !== "" || dependencies?.continue) {
      Question(
        dependencies.question,
        updateAnswer,
        addMessageOnFinish,
        dependencies.continue
      );
    }
  }, [dependencies]);

  async function addMessageOnFinish() {
    let lastMessage = null;

    do {
      await delay(250);
      lastMessage = await GetLastMessage();
    } while (
      lastMessage === null ||
      lastMessage?.unix === messages.slice(-1)[0].unix
    );

    const updated = [...messages.slice(0, -1), lastMessage];
    const uniques = updated.filter((message, index) => {
      return index === updated.findIndex((o) => message.id === o.id);
    });
    setMessages(uniques);
    setMessagesLocalStorage(uniques);
    setQuestionText("");
    setLoading(false);
    setContinueText(false);
  }

  const question = (text: string) => {
    try {
      setLoading(true);
      setMessages([...messages, createFakeMessage(text)]);
      setQuestionText(text);
      setContinueText(false);
    } catch (error) {
      auth.logout();
    }
  };

  const continueAnswer = () => {
    try {
      setLoading(true);
      setQuestionText("");
      setContinueText(true);
    } catch (error) {
      auth.logout();
    }
  };

  const updateAnswer = (message: string, fullMessage: string) => {
    const old = messages.slice(0, -1);
    const updated = messages.slice(-1)[0];
    updated.answer += message;
    const updatedMessages = [...old, updated];
    setMessages(updatedMessages);
    setMessagesLocalStorage(updatedMessages);
  };

  return (
    <MessageContext.Provider
      value={{
        messages,
        loading,
        setMessages,
        question,
        updateMessages,
        clearMessages,
        continueAnswer,
      }}
    >
      {children}
    </MessageContext.Provider>
  );
};
