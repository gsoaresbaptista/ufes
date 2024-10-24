const formattedTime = new Intl.DateTimeFormat("pt-BR", {
  hour: "2-digit",
  minute: "2-digit",
});



export function formatMessageTime(created_at: string) {
  // 60 * 60 * 3 * 1000 = (Brazil UTC-3, in ms)
  return formattedTime.format(
    Date.parse(created_at) - 60 * 60 * 3 * 1000
  );
}


export function formatMessage(message: any) {
    return {
        ...message,
        created_at: formatMessageTime(message.created_at),
        unix: new Date().getTime(),
    }
}