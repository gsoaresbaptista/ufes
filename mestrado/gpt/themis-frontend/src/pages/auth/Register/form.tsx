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

function RegisterForm({
  form,
  onSubmit,
}: {
  form: any;
  onSubmit: any;
}) {
  return (
    <div className="w-full flex flex-col justify-center items-center">
      <div className="p-4 w-3/4 md:w-2/4">
        <Form {...form}>
          <form onSubmit={form.handleSubmit(onSubmit)} className="space-y-8">
            <FormField
              control={form.control}
              name="name"
              render={({ field }) => (
                <FormItem className="space-y-0">
                  <FormLabel>Nome Completo</FormLabel>
                  <FormControl>
                    <Input
                      className="shad-input"
                      placeholder="Seu nome"
                      {...field}
                    />
                  </FormControl>
                  <FormMessage className="absolute" />
                </FormItem>
              )}
            />

            <FormField
              control={form.control}
              name="email"
              render={({ field }) => (
                <FormItem className="space-y-0">
                  <FormLabel>E-mail</FormLabel>
                  <FormControl>
                    <Input
                      className="shad-input"
                      placeholder="Seu e-mail"
                      {...field}
                    />
                  </FormControl>
                  <FormMessage className="absolute" />
                </FormItem>
              )}
            />

            <FormField
              control={form.control}
              name="password"
              render={({ field }) => (
                <FormItem className="space-y-0">
                  <FormLabel>Senha</FormLabel>
                  <FormControl>
                    <Input
                      type="password"
                      className="shad-input"
                      placeholder="Sua senha"
                      {...field}
                    />
                  </FormControl>
                  <FormMessage className="absolute" />
                </FormItem>
              )}
            />

            <FormField
              control={form.control}
              name="passwordCheck"
              render={({ field }) => (
                <FormItem className="space-y-0">
                  <FormLabel>Confirme sua senha</FormLabel>
                  <FormControl>
                    <Input
                      type="password"
                      className="shad-input"
                      placeholder="Confirme sua senha"
                      {...field}
                    />
                  </FormControl>
                  <FormMessage className="absolute" />
                </FormItem>
              )}
            />

            <Button type="submit" className="w-full dark:text-foreground">
              Cadastrar
            </Button>
          </form>
        </Form>
      </div>
      <div className="flex gap-2">
        Já possui uma conta?
        <Link to="/login" className="dark:text-primary text-violet-600">
          Entre agora!
        </Link>
      </div>
    </div>
  );
}

export default RegisterForm;
