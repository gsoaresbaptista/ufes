import { Outlet, Navigate } from "react-router-dom";

import TopBar from "../../components/shared/TopBar";
import SideBar from "../../components/shared/SideBar/SideBar";
import BottomBar from "../../components/shared/BottomBar";

import { useAuth } from "@/context/AuthProvider/useAuth";

import { ProtectedLayout } from "@/components/ProtectedLayout";

export default function RootLayout() {
  const auth = useAuth();

  return (
    <>
      <ProtectedLayout>
        {!auth?.uuid ? (
          <Navigate to="/login" />
        ) : (
          <div className="w-full flex flex-col md:flex-row min-h-full items-center">
            <TopBar />
            <SideBar />
            <Outlet />
            <BottomBar />
          </div>
        )}
      </ProtectedLayout>
    </>
  );
}
