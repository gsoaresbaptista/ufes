import { useNavigate } from "react-router-dom";
import { useForm } from "react-hook-form";

import * as z from "zod";

import { useAuth } from "../../../context/AuthProvider/useAuth";
import { LoginFormObject, LoginFormSchema } from "./schema";
import LoginForm from "./form";
import { useState } from "react";

export const Login = () => {
  const auth = useAuth();
  const navigate = useNavigate();
  const form = useForm<z.infer<typeof LoginFormSchema>>(LoginFormObject);
  const [error, setError] = useState(null as string | null);

  async function onSubmit(values: z.infer<typeof LoginFormSchema>) {
    const { email, password } = values;

    const response = await auth.authenticate(email, password);
    if (response) {
      navigate("/chat");
    } else {
      setError("E-mail ou senha incorretos. Por favor, tente novamente.");
    }
  }

  return <LoginForm form={form} onSubmit={onSubmit} error={error} />;
};
