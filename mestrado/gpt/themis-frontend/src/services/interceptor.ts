import { post } from "./api";
import {
  getUserLocalStorage,
  setUserLocalStorage,
} from "../context/AuthProvider/utils";

const { fetch: originalFetch } = window;

export async function errorHandler(...args: any) {
  let [resource, config] = args;
  let response = await originalFetch(resource, config);

  if (!response.ok && response.status === 401) {
    const user = getUserLocalStorage();
    const response = await post("/users/refresh-token", {
      "refresh-token": user?.refreshToken?.id,
    });

    if (response.status == 200) {
      const body = await response.json();

      setUserLocalStorage({
        ...user,
        accessToken: body?.data?.access_token,
        refreshToken: body?.data?.refresh_token,
      });
      const updatedConfigs = {
        ...config,
        headers: {
          ...config?.headers,
          Authorization: `Bearer ${body?.data?.access_token}`,
        },
      };

      return await originalFetch(resource, updatedConfigs);
    }

    return Promise.reject(response);
  }
  return response;
}
