import { createContext, useEffect, useState } from "react";
import { IContext, IAuthProvider, IUser } from "./types";
import {
  LoginRequest,
  getUserLocalStorage,
  setUserLocalStorage,
} from "./utils";

import { Navigate } from "react-router-dom";

export const AuthContext = createContext<IContext>({} as IContext);

export const AuthProvider = ({ children }: IAuthProvider) => {
  const [user, setUser] = useState<IUser | null>();

  useEffect(() => {
    const user = getUserLocalStorage();
    
    if (user) {
      // check refresh token validation
      const expires = new Date(user.refreshToken.expires_in);

      if (expires > new Date()) {
        setUser(user);
      } else {
        logout();
      }
    }
  }, []);

  async function authenticate(email: string, password: string) {
    try {
      const response = await LoginRequest(email, password);

      if (response?.status == 200) {
        const body = await response.json();
  
        const payload = {
          uuid: body.data?.user?.id,
          name: body.data?.user?.name,
          email: body.data?.user?.email,
          refreshToken: body.data?.refresh_token,
          accessToken: body.data?.access_token,
        };
  
        setUser(payload);
        setUserLocalStorage(payload);
  
        return true;
      }

      return false;
    } catch (error) {
      return false;
    }
  }

  async function logout() {
    setUser(null);
    setUserLocalStorage(null);
    Navigate({to: "/login"});
  }

  return (
    <AuthContext.Provider value={{ ...user, authenticate, logout }}>
      {children}
    </AuthContext.Provider>
  );
};
