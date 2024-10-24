import { post } from "../../services/api";
import { IUser } from "./types";

export function setUserLocalStorage(user: IUser | null) {
  localStorage.setItem("user", JSON.stringify(user));
}

export function getUserLocalStorage() {
  const json = localStorage.getItem("user");

  if (!json) {
    return null;
  }

  const user = JSON.parse(json);

  return user ?? null;
}

export async function LoginRequest(email: string, password: string) {
  try {
    const request = post("/users/sign-in", {
      email: email,
      password: password,
    });

    return request;
  } catch (error) {
    return null;
  } 
}
