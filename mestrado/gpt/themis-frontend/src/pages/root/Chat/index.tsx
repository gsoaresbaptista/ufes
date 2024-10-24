import { useMessageContext } from "@/context/MessagesProvider/useMessages";
import React, { useEffect, useRef, useState } from "react";
import { Textarea } from "@/components/ui/textarea";
import UserIcon from "@/components/icons/UserIcon";
import LotusIcon from "@/components/icons/LotusIcon";
import { SendIcon } from "lucide-react";
import { Button } from "@/components/ui/button";

const Chat = () => {
  const { messages, loading, question, continueAnswer } = useMessageContext();
  const [shouldScroll, setShouldScroll] = useState(true);
  const [questionInput, setQuestionInput] = useState<string>("");
  const messagesContainerRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    const container = messagesContainerRef.current;
    if (container && shouldScroll) {
      container.scrollTop = container.scrollHeight;
    }
  }, [messages, shouldScroll]);

  const handleScroll = () => {
    const container = messagesContainerRef.current;
    const atBottom =
      container!.scrollHeight - container!.scrollTop ===
      container!.clientHeight;
    setShouldScroll(atBottom);
  };

  const handleQuestionClick = async () => {
    if (!loading && questionInput !== "") {
      setQuestionInput("");
      await question(questionInput);
      if (messagesContainerRef.current) {
        messagesContainerRef.current.scrollTop =
          messagesContainerRef.current.scrollHeight;
      }
    }
  };

  const handleContinueClick = async () => {
    if (!loading) {
      setQuestionInput("");
      await continueAnswer();
      if (messagesContainerRef.current) {
        messagesContainerRef.current.scrollTop =
          messagesContainerRef.current.scrollHeight;
      }
    }
  };

  const handleKeyDown = async (event: React.KeyboardEvent) => {
    if (event.key === "Enter" && !event.shiftKey) {
      event.preventDefault();
      await handleQuestionClick();
    }
  };

  return (
    <div className="flex flex-col w-full h-full md:h-screen justify-center overflow-x-hidden lg:flex-1">
      <div className="flex flex-col max-h-full items-center box-border py-5 px-10 xl:px-0 min-h-[65vh] lg:min-h-[100vh]">
        <div
          className="overflow-y-scroll overflow-x-hidden pr-8 xl:max-w-3xl xl:min-w-[48rem] max-w-xl min-w[36rem] gap-8 flex flex-col flex-1 min-h-3/4"
          ref={messagesContainerRef}
          onScroll={handleScroll}
        >
          {messages.length > 0 ? (
            messages?.map((message: any) => (
              <div key={message.id} className="w-full flex flex-col gap-4">
                <div className="flex gap-4 items-start">
                  <div className="bg-secondary/75 p-2 rounded-full">
                    <UserIcon />
                  </div>
                  <div className="userMessage message">
                    <p>{message.question}</p>
                    <p className="message-date">{message.created_at}</p>
                  </div>
                </div>
                <div className="flex items-end gap-4 w-full">
                  <div className="botMessage message">
                    {message.answer.replace(/<0x0A>/g, '\n').replace(/^\n/, '')
                      .split("\n")
                      .map((line: string, index: number) => (
                        <React.Fragment key={index}>
                          {line}
                          {index < message.answer.length - 1 && <br />}
                        </React.Fragment>
                      ))}
                    <p className="message-date">{message.created_at}</p>
                  </div>
                  <div className="bg-secondary/75 p-2 rounded-full">
                    <LotusIcon
                      width={28}
                      height={28}
                      className="fill-primary"
                    />
                  </div>
                </div>
              </div>
            ))
          ) : (
            <div className="flex flex-col justify-center items-center flex-1">
              <div className="dark:bg-white bg-primary w-fit p-2 rounded-full">
                <LotusIcon
                  width={80}
                  height={80}
                  className="dark:fill-black fill-white"
                />
              </div>
              <h2 className="mt-5 text-xl">Como posso ajudar você hoje?</h2>
            </div>
          )}
          {messages.length > 0 && (
            <div className="continue-container">
              <Button
                className="continue-btn"
                onClick={async () => await handleContinueClick()}
                disabled={loading}
              >
                Continuar
              </Button>
            </div>
          )}
        </div>
        <div className="flex gap-1 max-h-[100px] mt-10 md:max-w-2xl w-[80%] relative">
          <div className="w-full relative">
            <Textarea
              className="question-input rounded-xl resize-none"
              onKeyDown={async (event) => await handleKeyDown(event)}
              onChange={(event) => setQuestionInput(event.target.value)}
              value={questionInput}
              disabled={loading}
            />
            <div
              className={
                "flex items-center justify-center absolute right-3 bottom-0 h-full" +
                (loading || questionInput === "" ? "" : " cursor-pointer")
              }
              onClick={async () => await handleQuestionClick()}
            >
              <SendIcon
                className={loading ? "stroke-primary/25" : "stroke-primary/75"}
              />
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Chat;
