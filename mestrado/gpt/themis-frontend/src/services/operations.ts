import { get, post, delete_ } from "./api";
import { formatMessage } from "./utils";

export async function LoadMessages() {
  const response = await get("/messages");
  const data = await response.json();
  const messages = data.data.messages.map(formatMessage);
  return messages;
}

export async function GetLastMessage() {
  const response = await get("/messages/last");
  const data = await response.json();

  if (data.data.message) {
    const message = formatMessage(data.data.message);
    return message;
  }

  return null;
}

export async function CreateUser({
  name,
  email,
  password,
  passwordCheck,
}: {
  name: string;
  email: string;
  password: string;
  passwordCheck: string;
}) {
  return await post("/users/sign-up", { name, email, password, passwordCheck });
}

export async function Question(
  question: string,
  callback: (msg: string, fullMessage: string) => void,
  onFinish?: (fullMessage: string) => any,
  continueAnswer?: boolean
) {
  let fullMessage = "";
  const postPromise = continueAnswer
    ? post("/messages/continue", {})
    : post("/messages/question", { question: question });

  await postPromise.then((response) => {
    const reader = response?.body?.getReader();

    async function processStream({
      done,
      value,
    }: ReadableStreamReadResult<Uint8Array>) {
      if (done) {
        if (onFinish) {
          await onFinish(fullMessage);
        }
        return;
      }

      const message = new TextDecoder("utf-8").decode(value);
      fullMessage += message;
      callback(message, fullMessage);

      // continue reading
      reader?.read().then(processStream);
    }

    // start reading
    reader?.read().then(processStream);
  });
}

export async function Clear() {
  try {
    const response = await delete_("/messages/clear-chat");
    const messages = (await response.json()).data.messages;
    return messages;
  } catch (error) {
    return [];
  }
}
