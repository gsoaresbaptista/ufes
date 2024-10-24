import { AlertCircle } from "lucide-react";

import { Link } from "react-router-dom";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import {
  Form,
  FormControl,
  FormField,
  FormItem,
  FormLabel,
  FormMessage,
} from "@/components/ui/form";

import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert";

function LoginForm({
  form,
  onSubmit,
  error,
}: {
  form: any;
  onSubmit: any;
  error: string | null;
}) {
  const handleSubmit = async (formData: any) => {
    await onSubmit(formData);
  };

  return (
    <div className="w-full flex flex-col justify-center items-center">
      {error && (
        <div className="w-3/5 pb-3">
          <Alert variant="destructive">
            <AlertCircle className="h-4 w-4" />
            <AlertTitle>Error</AlertTitle>
            <AlertDescription>{error}</AlertDescription>
          </Alert>
        </div>
      )}
      <div className="p-4 w-3/4 md:w-2/4 flex flex-col">
        <Form {...form}>
          <form
            onSubmit={form.handleSubmit(handleSubmit)}
            className="space-y-8"
          >
            <div className="grid w-full items-center gap-4">
              <FormField
                control={form.control}
                name="email"
                render={({ field }) => (
                  <FormItem>
                    <FormLabel>E-mail</FormLabel>
                    <FormControl>
                      <Input
                        className="shad-input"
                        placeholder="Seu e-mail"
                        {...field}
                      />
                    </FormControl>
                    <FormMessage />
                  </FormItem>
                )}
              />

              <FormField
                control={form.control}
                name="password"
                render={({ field }) => (
                  <FormItem>
                    <FormLabel>Senha</FormLabel>
                    <FormControl>
                      <Input
                        type="password"
                        className="shad-input"
                        placeholder="Sua senha"
                        {...field}
                      />
                    </FormControl>
                    <FormMessage />
                  </FormItem>
                )}
              />
            </div>
            <Button type="submit" className="w-full dark:text-foreground">
              Entrar
            </Button>
          </form>
        </Form>
      </div>
      <div className="flex gap-2">
        Ainda não tem uma conta?
        <Link to="/register" className="dark:text-primary text-violet-600">
          Registre-se
        </Link>
      </div>
    </div>
  );
}

export default LoginForm;
