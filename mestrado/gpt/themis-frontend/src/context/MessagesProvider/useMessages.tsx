import { useContext } from "react";
import { MessageContext } from ".";

export const useMessageContext = () => {
  const context = useContext(MessageContext);
  return context;
};
