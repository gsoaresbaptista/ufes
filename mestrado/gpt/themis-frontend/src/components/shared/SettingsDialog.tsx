import React, { useState } from "react";
import { Button } from "../ui/button";

import { useTheme } from "../ui/theme-provider";
import { Dialog, DialogContent, DialogHeader, DialogTitle } from "../ui/dialog";
import ClearDialog from "./ClearDialog";

import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "../ui/select";
import { useMessageContext } from "@/context/MessagesProvider/useMessages";

interface SettingsDialogProps {
  open: boolean;
  setOpen: React.Dispatch<React.SetStateAction<boolean>>;
}

const SettingsDialog: React.FC<SettingsDialogProps> = ({ open, setOpen }) => {
  const [tab, setTab] = useState("general");
  const [clear, setClear] = useState(false);
  const { theme, setTheme } = useTheme();
  const data = useMessageContext();

  const itemClass = (name: string) =>
    `tabButton ${name === tab ? "active" : "inactive"}`;

  return (
    <Dialog open={open} onOpenChange={setOpen}>
      <DialogContent>
        <DialogHeader>
          <DialogTitle>Configurações</DialogTitle>
        </DialogHeader>
        <div className="flex gap-6">
          <ul className="w-1/3 flex flex-col">
            <li className="tabItem">
              <button
                className={itemClass("general")}
                onClick={() => setTab("general")}
              >
                Geral
              </button>
            </li>
            <li className="tabItem">
              <button
                className={itemClass("themis")}
                onClick={() => setTab("themis")}
              >
                Conversa
              </button>
            </li>
          </ul>
          <div className="flex-1">
            {tab === "general" ? (
              <div className="flex flex-col gap-4">
                <div className="flex justify-between items-center">
                  <p>Tema</p>
                  <Select
                    onValueChange={(value) => setTheme(value as any)}
                    defaultValue={theme}
                  >
                    <SelectTrigger className="w-[180px]">
                      <SelectValue placeholder="Theme" />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="light">Claro</SelectItem>
                      <SelectItem value="dark">Escuro</SelectItem>
                      <SelectItem value="system">Sistema</SelectItem>
                    </SelectContent>
                  </Select>
                </div>
                <div className="flex justify-between items-center">
                  <p>Limpar toda conversa</p>
                  <Button
                    variant="destructive"
                    onClick={() => setClear(true)}
                    disabled={data.loading}
                  >
                    Limpar
                  </Button>
                  <ClearDialog open={clear} setOpen={setClear} />
                </div>
              </div>
            ) : (
              <div>
                <div className="flex justify-between items-center">Empty</div>
              </div>
            )}
          </div>
        </div>
      </DialogContent>
    </Dialog>
  );
};

export default SettingsDialog;
