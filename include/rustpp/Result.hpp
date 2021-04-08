#pragma once

#include <stdexcept>
#include <variant>
#include <optional>
#include "config.hpp"
#include "match.hpp"
#include "types.hpp"

namespace rustpp {

    namespace muties {

        template<typename T>
        struct Ok
        {
            T value;
        };

        template<typename E>
        struct Err
        {
            E value;
        };

        struct None {};

        template<typename T, typename E>
        struct Result
        {

            Result(T okvalue) noexcept : value{ Ok<T>{okvalue} } {}
            Result(Err<E> errvalue) noexcept : value{ Err<E>{errvalue} }, panic_on_dtor{ true } {}
            Result(const Result &) = delete;
            Result(Result &&other) noexcept : value{ std::move(other.value) } { other._consume(); }
            Result& operator=(const Result &) = delete;
            Result &operator=(Result &&other) noexcept { value = std::move(other.value); other._consume(); }
            ~Result() noexcept(false)
            {
                if (panic_on_dtor) {
                    throw std::runtime_error("Panic!");
                }
            }

            auto is_ok() const -> rool {
                return value.index() == 0;
            }

            auto is_err() const -> rool {
                return value.index() == 1;
            }

            auto ok() const -> std::optional<T> {
                _consume();
                if (is_ok()) {
                    return std::optional<T>{_okval()};
                } else {
                    return {};
                }
            }

            auto err() const -> std::optional<T> {
                _consume();
                if (is_err()) {
                    return std::optional<E>{_errval()};
                } else {
                    return {};
                }
            }

            template<typename U>
            auto and_(const Result<U, E> &res) const -> const Result<U, E> {
                if (is_ok()) {
                    return res;
                } else {
                    return _errobj();
                }
            }

            template<typename U>
            auto and_then(const Result<U, E>(*fn)(T)) -> const Result<U, E> {
                if (is_ok()) {
                    return fn(_okval());
                } else {
                    return _errobj();
                }
            }

            template<typename F>
            auto or_(const Result<T, F> &res) const -> const Result<T, F> {
                if (is_err()) {
                    return res;
                } else {
                    return _okobj();
                }
            }

            template<typename F>
            auto or_else(const Result<T, F>(*fn)(E)) -> const Result<T, F> {
                if (is_err()) {
                    return fn(_errval());
                } else {
                    return _okobj();
                }
            }

            auto unwrap_or(T def) const -> T {
                if (is_ok()) {
                    return _okval();
                } else {
                    return def;
                }
            }

            auto unwrap_or_else(T(*closure)(E)) const -> T {
                if (is_ok()) {
                    return _okval();
                } else {
                    return closure(_errval());
                }
            }

            auto expect(String &msg) const -> T {
                _consume();
                if (is_err()) {
                    throw std::runtime_error(msg);
                }
                return _okval();
            }

            auto unwrap() const -> T {
                _consume();
                if (is_err()) {
                    throw std::runtime_error(_errval());
                }
                return _okval();
            }

            auto expect_err(String &msg) const -> E {
                _consume();
                if (is_ok()) {
                    throw std::runtime_error(msg + _okval());
                }
                return _errval();
            }
            
            auto unwrap_or_default() const -> T {
                _consume();
                if (is_ok()) {
                    return _okval();
                } else {
                    return {};
                }
            }

            template<typename ...Args>
            friend struct match;

            // private but not because I can't get friends working in clang yet :(

            auto _okval() const -> T& { return std::get<0>(value).value; }
            auto _errval() const -> E& { return std::get<1>(value).value; }
            auto _okobj() const -> const Ok<T> &{ return std::get<0>(value); }
            auto _errobj() const -> const Err<E> &{ return std::get<1>(value); }
            auto _consume() const { panic_on_dtor = false; }


        private:

            std::variant<Ok<T>, Err<E>> value;
            mutable bool panic_on_dtor{};


        };

    } // namesapce muties.

    template<typename T, typename E>
    using Result = const muties::Result<T, E>;

    template<typename T>
    using Ok = const muties::Ok<T>;

    template<typename E>
    using Err = const muties::Err<E>;

    using None = const muties::None;

    template<typename T, typename E>
    struct match<Result<T,E>>
    {
        Result<T, E> &result;

        match(Result<T, E> &resulta) : result{ resulta } {}

        template<typename FnOk, typename FnErr>
        auto operator()(FnOk fnok, FnErr fnerr) const {
            result._consume();
            if (result.is_ok()) {
                return fnok(result._okobj());
            } else {
                return fnerr(result._errobj());
            }
        }
    };

    template<typename T, typename E> match(Result<T, E>)->match<Result<T, E>>;

} // namespace rustpp.