import { Outlet, Navigate } from "react-router-dom";
import { useAuth } from "../../context/AuthProvider/useAuth";
import Logo from "@/components/shared/Logo";

export default function AuthLayout() {
  const auth = useAuth();

  return (
    <>
      {auth?.uuid ? (
        <Navigate to="/chat" />
      ) : (
        <div className="bg-secondary flex w-full h-full justify-center items-center ">
          <div className="flex flex-col md:w-3/4 md:h-3/5 md:min-h-[600px] w-full h-full bg-background rounded-[1.5rem]">
            <div className="p-5 absolute">
              <Logo bigVersion={false} />
            </div>
            <div className="flex w-full h-full justify-center items-center">
              <section className="flex flex-1 justify-center items-center flex-col py-10">
                <Outlet />
              </section>

              <img
                src="/assets/images/side-img.jpg"
                alt="themis statue"
                className="hidden xl:block w-2/5 h-full object-cover  p-2 rounded-[1.5rem]"
              />
            </div>
          </div>
        </div>
      )}
    </>
  );
}
