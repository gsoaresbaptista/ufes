import { UserCheck } from "lucide-react";
import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert";

import { useNavigate } from "react-router-dom";

import { useForm } from "react-hook-form";
import * as z from "zod";

import { RegisterFormObject, RegisterFormSchema } from "./schema";
import RegisterForm from "./form";
import { CreateUser } from "@/services/operations";
import { useEffect, useState } from "react";
import { Link } from "react-router-dom";

export const Register = () => {
  const navigate = useNavigate();
  const [created, setCreated] = useState(false);
  const [seconds, setSeconds] = useState(5);
  const [redirect, setRedirect] = useState(false);
  const form: any =
    useForm<z.infer<typeof RegisterFormSchema>>(RegisterFormObject);

  async function onSubmit(data: z.infer<typeof RegisterFormSchema>) {
    const response = await CreateUser(data);

    if (response.status === 201) {
      setCreated(true);
    }
  }

  useEffect(() => {
    if (created) {
      const timer = setInterval(() => {
        setSeconds((prevTime) => {
          if (prevTime <= 0) {
            clearInterval(timer);
            setRedirect(true);
          }
          return prevTime - 1;
        });
      }, 1000);

      return () => clearInterval(timer);
    }
  }, [created, navigate]);

  useEffect(() => {
    if (redirect) {
      navigate("/login", { replace: true });
    }
  }, [redirect]);

  return (
    <>
      {created ? (
        <div className="w-3/4">
          <Alert className="bg-secondary">
            <UserCheck className="h-4 w-4" />
            <AlertTitle>Usuário Criado com Sucesso!</AlertTitle>
            <AlertDescription>
              Aguarde um momento, pois você será redirecionado para bater um
              papo com a Themis em {seconds} segundos.
            </AlertDescription>
          </Alert>
          <p className="justify-center mt-3 flex gap-2">
            Está com presa?
            <Link to="/login" className="text-primary font-bold">
              Clique aqui!
            </Link>
          </p>
        </div>
      ) : (
        <RegisterForm onSubmit={onSubmit} form={form} />
      )}
    </>
  );
};
