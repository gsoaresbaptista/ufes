import { AuthProvider } from "./context/AuthProvider";
import { MessageProvider } from "./context/MessagesProvider";
import { BrowserRouter, Route, Routes, Navigate } from "react-router-dom";
import { Register } from "./pages/auth/Register";
import { Login } from "./pages/auth/Login";
import Chat from "./pages/root/Chat";
import AuthLayout from "./pages/auth/AuthLayout";
import { ThemeProvider } from "./components/ui/theme-provider";

import "./global.css";
import RootLayout from "./pages/root/RootLayout";
import About from "./pages/root/About";
import Code from "./pages/root/Code";

function App() {
  return (
    <main className="flex flex-col h-screen">
      <AuthProvider>
        <MessageProvider>
          <ThemeProvider defaultTheme="dark" storageKey="vite-ui-theme">
            <BrowserRouter>
              <Routes>
                {/* public routes */}
                <Route element={<AuthLayout />}>
                  <Route path="/" element={<Navigate to='/login' />} />
                  <Route path="/login" element={<Login />} />
                  <Route path="/register" element={<Register />} />
                </Route>

                {/* private routes */}
                <Route element={<RootLayout />}>
                  <Route path="/chat" element={<Chat />} />
                  <Route path="/about" element={<About />} />
                  <Route path="/code" element={<Code />} />
                </Route>
              </Routes>
            </BrowserRouter>
          </ThemeProvider>
        </MessageProvider>
      </AuthProvider>
    </main>
  );
}

export default App;
