import * as z from "zod";
import { zodResolver } from "@hookform/resolvers/zod";

export const LoginFormSchema = z.object({
  email: z.string().email("E-mail inválido."),
  password: z.string().min(1, "Não pode ser vazio."),
});

export const LoginFormObject = {
  resolver: zodResolver(LoginFormSchema),
  defaultValues: {
    email: "",
    password: "",
  },
};
