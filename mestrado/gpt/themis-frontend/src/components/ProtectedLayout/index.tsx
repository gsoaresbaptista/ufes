import { useAuth } from "../../context/AuthProvider/useAuth";
import { Navigate } from "react-router-dom";

export const ProtectedLayout = ({ children }: { children: JSX.Element }) => {
  const auth = useAuth();

  if (!auth.uuid) {
    <Navigate to='/login' />
  }

  return children;
};
