export interface IUser {
    uuid?: string;
    name?: string;
    email?: string;
    accessToken?: string;
    refreshToken?: string;
}

export interface IContext extends IUser {
    authenticate: (email: string, password: string) => Promise<boolean>;
    logout: () => void;
}

export interface IAuthProvider {
    children: JSX.Element;
}
