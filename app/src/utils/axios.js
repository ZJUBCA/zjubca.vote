import axios from 'axios';

const instance = axios.create({
  baseURL: 'https://api.github.com',
  timeout: 30000,
  headers: {
    Accept: 'application/vnd.github.v3+json, application/vnd.github.symmetra-preview+json',
  }
});

export default instance;
