import {
  getUserLocalStorage,
} from "../context/AuthProvider/utils";
import { errorHandler } from "./interceptor";


const baseURL = 'http://localhost:8000';

window.fetch = errorHandler;

export async function post(endpoint: string, data: object) {
  const url = `${baseURL}${endpoint}`;
  const user = getUserLocalStorage();

  return await fetch(url, {
    method: "POST",
    body: JSON.stringify(data),
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${user?.accessToken}`,
    },
  });
}

export async function get(endpoint: string) {
  const url = `${baseURL}${endpoint}`;
  const user = getUserLocalStorage();

  return await fetch(url, {
    method: "GET",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${user?.accessToken}`,
    },
  });
}

export async function delete_(endpoint: string) {
  const url = `${baseURL}${endpoint}`;
  const user = getUserLocalStorage();

  return await fetch(url, {
    method: "DELETE",
    headers: {
      "Content-Type": "application/json",
      Authorization: `Bearer ${user?.accessToken}`,
    },
  });
}