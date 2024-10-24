import * as z from "zod";
import { zodResolver } from "@hookform/resolvers/zod";

const passwordType = z
  .string()
  .min(8, "Deve ter pelo menos 8 caracteres.")
  .refine((password) => /[a-z]/.test(password), {
    message: "Deve ter pelo menos um caractere minúsculo.",
  })
  .refine((password) => /[A-Z]/.test(password), {
    message: "Deve ter pelo menos um caractere maiúsculo.",
  })
  .refine((password) => /[+!@#$%^&*(),.?":{}|<>~'"]/.test(password), {
    message: "Deve ter pelo menos um caractere especial.",
  });

export const RegisterFormSchema = z
  .object({
    name: z
      .string()
      .min(4, {
        message: "O nome deve possuir pelo menos 4 caracteres.",
      })
      .refine(
        (name) => name.split(" ").length > 1,
        "Por favor, digite seu nome completo."
      ),
    email: z.string().email("E-mail inválido."),
    password: passwordType,
    passwordCheck: z.string().min(1, "Não pode ser vazio."),
  })
  .refine((data) => data.password === data.passwordCheck, {
    message: "As senhas não coincidem!",
    path: ["passwordCheck"],
  });

export const RegisterFormObject = {
  resolver: zodResolver(RegisterFormSchema),
  defaultValues: {
    name: "",
    email: "",
    password: "",
    passwordCheck: "",
  },
};
