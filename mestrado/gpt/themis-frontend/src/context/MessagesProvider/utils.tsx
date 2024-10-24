import { Message } from "./types";

export function setMessagesLocalStorage(messages: Message[]) {
  localStorage.setItem("messages", JSON.stringify(messages));
}

export function getMessagesLocalStorage() {
  const json = localStorage.getItem("messages");

  if (!json) {
    return null;
  }

  const messages = JSON.parse(json);

  return messages ?? [];
}

export function formatDate(date: Date) {
  const hours = String(date.getHours()).padStart(2, "0");
  const minutes = String(date.getMinutes()).padStart(2, "0");
  return `${hours}:${minutes}`;
}
